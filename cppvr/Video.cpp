#include "Video.h"

extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
}

Video::Video(const std::string& file) :
	m_isInitialized(false), m_file(file), m_currentVideoTime(0.0),
	m_currentAudioTime(0.0), m_lastAudioDts(0.0), m_lastAudioDelay(0.0),
	m_size(), m_formatContext(nullptr),
	m_videoDecoder(nullptr), m_videoDecoderContext(nullptr), m_swsContext(nullptr),
	m_audioDecoder(nullptr), m_audioDecoderContext(nullptr),
	m_videoStream(nullptr), m_audioStream(nullptr), 
	m_hasVideoData(false), m_videoBuffer(nullptr), m_videoBufferFrameData(nullptr),
	m_hasAudioData(false),  m_packet()
{
}

Video::~Video()
{
	if (!m_isInitialized) {
		return;
	}

	while (!m_videoQueue.empty()) {
		av_frame_free(&m_videoQueue.back());
		m_videoQueue.pop_back();
	}

	av_frame_free(&m_videoBuffer);
	av_free(m_videoBufferFrameData);

	avformat_close_input(&m_formatContext);

	avcodec_free_context(&m_videoDecoderContext);
	avcodec_free_context(&m_audioDecoderContext);

	avformat_network_deinit();
	sws_freeContext(m_swsContext);
}

void Video::init()
{
	if (m_isInitialized) {
		return;
	}

	// Connect to data stream

	if (avformat_open_input(&m_formatContext, m_file.data(), nullptr, nullptr) < 0) {
		throw std::runtime_error("Could not open input file\n");
	}
	if (avformat_find_stream_info(m_formatContext, nullptr) < 0) {
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
}

void Video::receive()
{
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

			std::lock_guard<std::mutex> lock(m_audioQueueMutex);
			m_audioQueue.push_back(frame);
		}
	}

	av_packet_unref(&m_packet);
}

bool Video::shouldReceive() const
{
	return m_videoQueue.size() < 256;
}

void Video::flush() const
{
	avcodec_send_packet(m_videoDecoderContext, nullptr);
	avcodec_send_packet(m_audioDecoderContext, nullptr);
}

void Video::decodeVideo()
{
	if (m_videoQueue.empty()) {
		return;
	}

	AVFrame* frame = m_videoQueue.front();

	if (frame == nullptr) {
		return;
	}

	if (m_currentVideoTime == 0.0f) {
		m_currentVideoTime = frame->best_effort_timestamp * av_q2d(m_videoStream->time_base) + 1.0f;
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
	if (m_audioQueue.empty()) {
		return;
	}

	AVFrame* frame = m_audioQueue.front();

	if (frame == nullptr) {
		return;
	}

	m_currentAudioTime += static_cast<double>(m_audioTimer.restart().asSeconds());

	const auto nextDts = m_lastAudioDts + m_lastAudioDelay;
	if (m_currentAudioTime < nextDts) {
		return;
	}

	m_lastAudioDts = nextDts;
	m_lastAudioDelay = frame->pkt_duration * av_q2d(m_videoStream->time_base);

	printf("Delay: %f\n", m_lastAudioDelay);

	int linesize;
	uint8_t** samples;
	av_samples_alloc(samples, &linesize, frame->channels,
		frame->nb_samples, frame->format, 1);

	int dataSize = av_samples_get_buffer_size(nullptr,
		m_audioDecoderContext->channels, m_audioDecoderContext->sample_rate,
		m_audioDecoderContext->sample_fmt, 1);

	m_audioBuffer.resize(dataSize);
	std::memcpy(&m_audioBuffer[0], &frame->data[0], 2);

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

bool Video::hasData() const
{
	return m_hasVideoData;
}

bool Video::writeData(uint8_t* destination, size_t size)
{
	if (!m_hasVideoData) return false;

	std::lock_guard<std::mutex> lock(m_videoBufferMutex);

	std::memcpy(destination, m_videoBuffer->data[0], size);
	m_hasVideoData = false;

	return true;
}

AudioStream& Video::getAudioStream()
{
	return m_soundStream;
}

void Video::initVideoStream()
{
	if (m_videoStream == nullptr) {
		throw std::runtime_error("Stream doesn't contain video\n");
	}

	printf("Video codec id: %d\n", m_videoStream->codecpar->codec_id);

	m_videoDecoder = avcodec_find_decoder(m_videoStream->codecpar->codec_id);
	if (!m_videoDecoder) {
		throw std::runtime_error("Can't find video decoder!\n");
	}

	m_videoDecoderContext = avcodec_alloc_context3(m_videoDecoder);

	if (avcodec_parameters_to_context(m_videoDecoderContext, m_videoStream->codecpar) < 0) {
		throw std::runtime_error("Failed to provide parameters\n");
	}

	if (avcodec_open2(m_videoDecoderContext, m_videoDecoder, nullptr) < 0) {
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
		throw std::runtime_error("Stream doesn't contain audio\n");
	}

	printf("Audio codec id: %d\n", m_audioStream->codecpar->codec_id);

	m_audioDecoder = avcodec_find_decoder(m_audioStream->codecpar->codec_id);
	if (!m_audioDecoder) {
		throw std::runtime_error("Can't find audio decoder!\n");
	}

	m_audioDecoderContext = avcodec_alloc_context3(m_audioDecoder);

	if (avcodec_parameters_to_context(m_audioDecoderContext, m_audioStream->codecpar) < 0) {
		throw std::runtime_error("Failed to provide parameters\n");
	}

	if (avcodec_open2(m_audioDecoderContext, m_audioDecoder, nullptr) < 0) {
		throw std::runtime_error("Unable to open audio decoder\n");
	}

	// Init sound

	m_soundStream.initialize(m_audioDecoderContext->channels, m_audioDecoderContext->sample_rate);
}
