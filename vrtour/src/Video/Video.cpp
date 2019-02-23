#include "Video/Video.h"

#include <asio/post.hpp>

extern "C" {
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libavutil/imgutils.h>
}

#include <Core/Core.h>

Video::Video(const ej::Core& core, const std::string& file):
	m_file(file)
{
	m_videoManager = core.get<VideoManager>();
}

Video::~Video()
{
	clear();
}

void Video::init()
{
	if (m_isInitialized) {
		return;
	}

	// Post initialization to video manager
	post(*m_videoManager->getService(), std::bind(&Video::initializationTask, this));
}

bool Video::shouldReceive() const
{
	return m_videoQueue.size() < 256;
}

void Video::flush() const
{
	if (!m_isInitialized) {
		return;
	}

	avcodec_send_packet(m_videoDecoderContext, nullptr);
	avcodec_send_packet(m_audioDecoderContext, nullptr);
}

void Video::decodeVideo()
{
	std::unique_lock<std::mutex> lock(m_videoDecoderMutex);

	if (!m_isInitialized) {
		return;
	}

	if (m_videoQueue.empty()) {
		return;
	}

	AVFrame* frame = m_videoQueue.front();

	if (frame == nullptr) {
		return;
	}

	if (!m_videoStarted) {
		m_currentVideoTime = m_videoStream->start_time * av_q2d(m_videoStream->time_base);
		m_videoTimer.restart();
		m_videoStarted = true;
	}
	else {
		m_currentVideoTime += static_cast<double>(m_videoTimer.restart().asSeconds());		
	}

	auto seconds = 0.0f;
	if (frame->pkt_dts != AV_NOPTS_VALUE) {
		seconds = frame->best_effort_timestamp * av_q2d(m_videoStream->time_base);
	}

	if (m_currentVideoTime < seconds) {
		return;
	}
	
	{
		std::lock_guard<std::mutex> lock(m_videoQueueMutex);
		m_videoQueue.pop_front();
	}

	{
		std::lock_guard<std::mutex> lock(m_videoBufferMutex);
		sws_scale(m_swsContext, frame->data, frame->linesize, 0, m_videoDecoderContext->height,
			m_videoBuffer->data, m_videoBuffer->linesize);
	}

	av_frame_free(&frame);

	m_hasVideoData = true;
}

void Video::decodeAudio()
{
	std::unique_lock<std::mutex> lock(m_audioDecoderMutex);

	if (!m_isInitialized) {
		return;
	}

	if (m_audioQueue.empty()) {
		return;
	}

	AVFrame* frame = m_audioQueue.front();

	if (frame == nullptr) {
		return;
	}

	if (!m_audioStarted) {
		m_lastAudioDts = m_audioStream->start_time * av_q2d(m_audioStream->time_base);
		m_audioStarted = true;
	}

	const auto nextDts = m_lastAudioDts + m_lastAudioDelay;
	if (m_currentVideoTime < nextDts) {
		return;
	}

	m_lastAudioDts = nextDts;
	m_lastAudioDelay = frame->pkt_duration * av_q2d(m_audioStream->time_base);

	int dataSize = av_samples_get_buffer_size(nullptr,
		m_audioDecoderContext->channels, frame->nb_samples,
		AV_SAMPLE_FMT_S16, 1);
	m_audioBuffer.resize(dataSize);

	auto dataPtr = m_audioBuffer.data();
	swr_convert(m_swrContext, &dataPtr, dataSize,
		const_cast<const uint8_t**>(frame->data), frame->nb_samples);

	{
		std::unique_lock<std::mutex> lock(m_audioQueueMutex);
		m_audioQueue.pop_front();
	}

	av_frame_free(&frame);

	m_hasAudioData = true;
}

glm::vec2 Video::getSize() const
{
	return m_size;
}

bool Video::hasVideoData() const
{
	return m_hasVideoData;
}

bool Video::writeVideoData(uint8_t* destination, size_t size)
{
	if (!m_hasVideoData) return false;

	std::lock_guard<std::mutex> lock(m_videoBufferMutex);

	std::memcpy(destination, m_videoBuffer->data[0], size);
	m_hasVideoData = false;

	return true;
}

bool Video::writeAudioData(const int16_t** destination, size_t& size)
{
	decodeAudio();

	if (!m_hasAudioData) return false;

	*destination = reinterpret_cast<int16_t*>(m_audioBuffer.data());
	size = m_audioBuffer.size() / 2;

	m_hasAudioData = false;
	return true;
}

void Video::initializationTask()
{
	// Connect to data stream

	AVDictionary* formatOptions = nullptr;
	av_dict_set(&formatOptions, "timeout", "60", 0);

	if (avformat_open_input(&m_formatContext, m_file.data(), nullptr, &formatOptions) < 0) {
		throw std::runtime_error("Could not open input file\n");
	}

	if (avformat_find_stream_info(m_formatContext, nullptr) < 0) {
		clear();
		throw std::runtime_error("Failed to retrieve input stream information\n");
	}

	printf("Successfully opened data stream %s\n", m_file.data());

	for (unsigned int i = 0; i < m_formatContext->nb_streams; i++) {
		const auto codecType = m_formatContext->streams[i]->codecpar->codec_type;
		printf("Stream: %d, Codec: %d\n", i, codecType);
		if (codecType == AVMEDIA_TYPE_VIDEO) {
			m_videoStream = m_formatContext->streams[i];
		}
		else if (codecType == AVMEDIA_TYPE_AUDIO) {
			m_audioStream = m_formatContext->streams[i];
		}
	}

	initVideoStream();
	initAudioStream();

	m_isInitialized = true;

	post(*m_videoManager->getService(), std::bind(&Video::receivingTask, this));
	post(*m_videoManager->getService(), std::bind(&Video::decodingTask, this));
}

void Video::receivingTask()
{
	if (!m_isInitialized) {
		return;
	}

	receive();

	post(*m_videoManager->getService(), std::bind(&Video::receivingTask, this));
}

void Video::decodingTask()
{
	if (!m_isInitialized) {
		return;
	}

	decodeVideo();

	post(*m_videoManager->getService(), std::bind(&Video::decodingTask, this));
}

void Video::initVideoStream()
{
	if (m_videoStream == nullptr) {
		clear();
		throw std::runtime_error("Stream doesn't contain video\n");
	}

	printf("Video codec id: %d\n", m_videoStream->codecpar->codec_id);

	m_videoDecoder = avcodec_find_decoder(m_videoStream->codecpar->codec_id);
	if (!m_videoDecoder) {
		clear();
		throw std::runtime_error("Can't find video decoder!\n");
	}

	m_videoDecoderContext = avcodec_alloc_context3(m_videoDecoder);

	if (avcodec_parameters_to_context(m_videoDecoderContext, m_videoStream->codecpar) < 0) {
		clear();
		throw std::runtime_error("Failed to provide parameters\n");
	}

	if (avcodec_open2(m_videoDecoderContext, m_videoDecoder, nullptr) < 0) {
		clear();
		throw std::runtime_error("Unable to open decoder\n");
	}

	// Create buffers
	if (m_videoDecoderContext->time_base.num > 1000 && m_videoDecoderContext->time_base.den == 1) {
		m_videoDecoderContext->time_base.den = 1000;
	}

	m_swsContext = sws_getContext(m_videoDecoderContext->width, m_videoDecoderContext->height,
		m_videoDecoderContext->pix_fmt, m_videoDecoderContext->width, m_videoDecoderContext->height,
		AV_PIX_FMT_RGB24, SWS_BICUBIC, nullptr, nullptr, nullptr);

	m_size = glm::vec2(m_videoDecoderContext->width, m_videoDecoderContext->height);

	av_init_packet(&m_packet);

	m_videoBuffer = av_frame_alloc();
	m_videoBuffer->width = m_videoDecoderContext->width;
	m_videoBuffer->height = m_videoDecoderContext->height;
	m_videoBuffer->format = AV_PIX_FMT_RGB24;

	const auto size = av_image_get_buffer_size(static_cast<AVPixelFormat>(m_videoBuffer->format),
		m_videoBuffer->width,
		m_videoBuffer->height,
		1);

	m_videoBufferFrameData = static_cast<uint8_t*>(av_malloc(size));

	av_image_fill_arrays(m_videoBuffer->data,
		m_videoBuffer->linesize,
		m_videoBufferFrameData,
		static_cast<AVPixelFormat>(m_videoBuffer->format),
		m_videoBuffer->width,
		m_videoBuffer->height,
		1);
}

void Video::initAudioStream()
{
	if (m_audioStream == nullptr) {
		clear();
		throw std::runtime_error("Stream doesn't contain audio\n");
	}

	printf("Audio codec id: %d\n", m_audioStream->codecpar->codec_id);

	m_audioDecoder = avcodec_find_decoder(m_audioStream->codecpar->codec_id);
	if (!m_audioDecoder) {
		clear();
		throw std::runtime_error("Can't find audio decoder!\n");
	}

	m_audioDecoderContext = avcodec_alloc_context3(m_audioDecoder);

	if (avcodec_parameters_to_context(m_audioDecoderContext, m_audioStream->codecpar) < 0) {
		clear();
		throw std::runtime_error("Failed to provide parameters\n");
	}

	if (avcodec_open2(m_audioDecoderContext, m_audioDecoder, nullptr) < 0) {
		clear();
		throw std::runtime_error("Unable to open audio decoder\n");
	}

	// Init sound

	m_swrContext = swr_alloc();
	av_opt_set_int(m_swrContext, "in_channel_count", m_audioDecoderContext->channels, 0);
	av_opt_set_int(m_swrContext, "out_channel_count", m_audioDecoderContext->channels, 0);
	av_opt_set_int(m_swrContext, "in_channel_layout", m_audioDecoderContext->channel_layout, 0);
	av_opt_set_int(m_swrContext, "out_channel_layout", m_audioDecoderContext->channel_layout, 0);
	av_opt_set_int(m_swrContext, "in_sample_rate", m_audioDecoderContext->sample_rate, 0);
	av_opt_set_int(m_swrContext, "out_sample_rate", m_audioDecoderContext->sample_rate, 0);
	av_opt_set_sample_fmt(m_swrContext, "in_sample_fmt", m_audioDecoderContext->sample_fmt, 0);
	av_opt_set_sample_fmt(m_swrContext, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
	swr_init(m_swrContext);

	m_soundStream.initialize(m_audioDecoderContext->channels, m_audioDecoderContext->sample_rate);

	m_soundStream.play();
}

void Video::receive()
{
	std::unique_lock<std::mutex> lock(m_receiverMutex);

	if (!m_isInitialized) {
		return;
	}

	if (av_read_frame(m_formatContext, &m_packet) < 0) {
		printf("Error: Unable to read frame\n");
		return;
	}

	if (m_packet.stream_index == m_videoStream->index) {
		if (avcodec_send_packet(m_videoDecoderContext, &m_packet) < 0) {
			throw std::runtime_error("Unable to send video packet for decoding");
		}

		AVFrame* frame = av_frame_alloc();

		if (avcodec_receive_frame(m_videoDecoderContext, frame) < 0) {
			av_frame_free(&frame);
		}
		else {
			//printf("Received video frame: %lld\n", frame->pts);

			std::lock_guard<std::mutex> lock(m_videoQueueMutex);
			m_videoQueue.push_back(frame);
		}
	}
	else if (m_packet.stream_index == m_audioStream->index) {
		if (avcodec_send_packet(m_audioDecoderContext, &m_packet) < 0) {
			printf("Unable to send audio packet for decoding\n");
			return;
			//throw std::runtime_error("Unable to send audio packet for decoding");
		}

		AVFrame* frame = av_frame_alloc();

		if (avcodec_receive_frame(m_audioDecoderContext, frame) < 0) {
			av_frame_free(&frame);
		}
		else {
			//printf("Received audio frame: %lld\n", frame->pts);

			{
				std::lock_guard<std::mutex> lock(m_audioQueueMutex);
				m_audioQueue.push_back(frame);
			}

			if (m_soundStream.getStatus() != sf::SoundStream::Status::Playing) {
				m_soundStream.play();
			}
		}
	}

	av_packet_unref(&m_packet);
}

void Video::clear()
{
	std::unique_lock<std::mutex> lockReceiver(m_receiverMutex);
	std::unique_lock<std::mutex> lockVideoDecoder(m_videoDecoderMutex);
	std::unique_lock<std::mutex> lockAudioDecoder(m_audioDecoderMutex);

	m_isInitialized = false;

	while (!m_videoQueue.empty()) {
		av_frame_free(&m_videoQueue.back());
		m_videoQueue.pop_back();
	}

	if (m_videoBuffer != nullptr) {
		av_frame_free(&m_videoBuffer);
	}

	if (m_videoBufferFrameData != nullptr) {
		av_free(m_videoBufferFrameData);
	}

	if (m_formatContext != nullptr) {
		avformat_close_input(&m_formatContext);
	}

	if (m_videoDecoderContext != nullptr) {
		avcodec_free_context(&m_videoDecoderContext);
	}

	if (m_audioDecoderContext != nullptr) {
		avcodec_free_context(&m_audioDecoderContext);
	}

	if (m_swsContext != nullptr) {
		sws_freeContext(m_swsContext);
	}
}
