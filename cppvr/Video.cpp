#include "Video.h"

extern "C" {
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
}

Video::Video(const std::string& file) :
	m_isInitialized(false), m_file(file), m_isPlaying(false), m_size(),
	m_currentVideoTime(0.0f),
	m_decoder(nullptr), m_decoderContext(nullptr), m_formatContext(nullptr), m_swsContext(nullptr),
	m_videoStreamIndex(0), m_hasData(false),
	m_buffer(nullptr), m_bufferFrameData(nullptr), m_packet()
{
}

Video::~Video()
{
	if (!m_isInitialized) {
		return;
	}

	while (!m_frameQueue.empty()) {
		av_frame_free(&m_frameQueue.back());
		m_frameQueue.pop_back();
	}

	av_frame_free(&m_buffer);
	av_free(m_bufferFrameData);

	avformat_close_input(&m_formatContext);
	avcodec_free_context(&m_decoderContext);

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

	// Initialize decoder from stream

	AVStream* stream = nullptr;
	for (unsigned int i = 0; i < m_formatContext->nb_streams; i++) {
		printf("Stream: %d, Codec: %d\n", i, m_formatContext->streams[i]->codecpar->codec_type);
		if (m_formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			m_videoStreamIndex = i;
			stream = m_formatContext->streams[i];
			break;
		}
	}
	if (stream == nullptr) {
		throw std::runtime_error("Stream doesn't contain video\n");
	}

	printf("Codec id: %d\n", stream->codecpar->codec_id);

	m_decoder = avcodec_find_decoder(stream->codecpar->codec_id);
	if (!m_decoder) {
		throw std::runtime_error("Can't find decoder!\n");
	}

	m_decoderContext = avcodec_alloc_context3(m_decoder);

	if (avcodec_parameters_to_context(m_decoderContext, stream->codecpar) < 0) {
		throw std::runtime_error("Failed to provide parameters\n");
	}

	if (avcodec_open2(m_decoderContext, m_decoder, nullptr) < 0) {
		throw std::runtime_error("Unable to open decoder\n");
	}

	m_swsContext = sws_getContext(m_decoderContext->width, m_decoderContext->height,
		m_decoderContext->pix_fmt, m_decoderContext->width, m_decoderContext->height,
		AV_PIX_FMT_RGB24, SWS_BICUBIC, nullptr, nullptr, nullptr);

	// Create buffers
	m_size = glm::vec2(m_decoderContext->width, m_decoderContext->height);

	av_init_packet(&m_packet);

	m_buffer = av_frame_alloc();
	m_buffer->width = m_decoderContext->width;
	m_buffer->height = m_decoderContext->height;
	m_buffer->format = AV_PIX_FMT_RGB24;

	const auto size = av_image_get_buffer_size(static_cast<AVPixelFormat>(m_buffer->format),
		m_buffer->width,
		m_buffer->height,
		1);

	m_bufferFrameData = static_cast<uint8_t*>(av_malloc(size));

	av_image_fill_arrays(m_buffer->data,
		m_buffer->linesize,
		m_bufferFrameData,
		static_cast<AVPixelFormat>(m_buffer->format),
		m_buffer->width,
		m_buffer->height,
		1);

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

	if (m_packet.stream_index == m_videoStreamIndex) {
		if (avcodec_send_packet(m_decoderContext, &m_packet) < 0) {
			throw std::runtime_error("Unable to send packet for decoding");
		}

		AVFrame* frame = av_frame_alloc();

		if (avcodec_receive_frame(m_decoderContext, frame) < 0) {
			av_frame_free(&frame);
		}
		else {
			//printf("Received frame: %lld\n", frame->pts);

			std::lock_guard<std::mutex> lock(m_frameQueueMutex);
			m_frameQueue.push_back(frame);
		}
	}

	av_packet_unref(&m_packet);
}

bool Video::shouldReceive() const
{
	return m_frameQueue.size() < 64;
}

void Video::flush() const
{
	avcodec_send_packet(m_decoderContext, nullptr);
}

void Video::decode()
{
	if (!m_isPlaying) {
		return;
	}

	m_currentVideoTime += m_timer.restart().asSeconds();

	if (m_frameQueue.empty()) {
		return;
	}

	AVFrame* frame = nullptr;

	{
		std::lock_guard<std::mutex> lock(m_frameQueueMutex);

		frame = m_frameQueue.front();

		if (frame == nullptr) {
			return;
		}

		AVStream* stream = m_formatContext->streams[m_videoStreamIndex];
		const auto seconds = (frame->pkt_dts - stream->start_time) * av_q2d(stream->time_base);

		//printf("Decoded frame: %f\n", seconds);

		if (m_currentVideoTime < seconds) {
			return;
		}

		m_frameQueue.pop_front();
	}

	std::lock_guard<std::mutex> lock(m_bufferMutex);

	sws_scale(m_swsContext, frame->data, frame->linesize, 0, m_decoderContext->height,
		m_buffer->data, m_buffer->linesize);

	av_frame_free(&frame);

	m_hasData = true;
}

void Video::play()
{
	if (m_isPlaying) {
		return;
	}

	m_isPlaying = true;
	m_timer.restart();
}

void Video::pause()
{
	if (!m_isPlaying) {
		return;
	}

	m_isPlaying = false;
	m_currentVideoTime += m_timer.restart().asSeconds();
}

bool Video::isPlaying() const
{
	return m_isPlaying;
}

glm::vec2 Video::getSize() const
{
	return m_size;
}

bool Video::hasData() const
{
	return m_hasData;
}

bool Video::writeData(uint8_t* destination, size_t size)
{
	if (!m_hasData) return false;

	std::lock_guard<std::mutex> lock(m_bufferMutex);

	std::memcpy(destination, m_buffer->data[0], size);
	m_hasData = false;

	return true;
}
