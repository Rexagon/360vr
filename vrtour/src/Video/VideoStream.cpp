#include "Video\VideoStream.h"

extern "C" {
#include <libavutil/imgutils.h>
}

VideoStream::VideoStream(VideoState& state, AVStream * stream) :
	m_state(state), m_stream(stream)
{
}

VideoStream::~VideoStream()
{
	clear();
}

void VideoStream::init()
{
	if (m_isInitialized) {
		return;
	}

	if (m_stream == nullptr || m_stream->codecpar->codec_type != AVMEDIA_TYPE_VIDEO) {
		throw std::runtime_error("Stream doesn't contain video\n");
	}

	printf("Video codec id: %d\n", m_stream->codecpar->codec_id);

	m_decoder = avcodec_find_decoder(m_stream->codecpar->codec_id);
	if (!m_decoder) {
		clear();
		throw std::runtime_error("Can't find video decoder!\n");
	}

	m_decoderContext = avcodec_alloc_context3(m_decoder);

	if (avcodec_parameters_to_context(m_decoderContext, m_stream->codecpar) < 0) {
		clear();
		throw std::runtime_error("Failed to provide parameters\n");
	}

	if (avcodec_open2(m_decoderContext, m_decoder, nullptr) < 0) {
		clear();
		throw std::runtime_error("Unable to open decoder\n");
	}

	// Create buffers
	if (m_decoderContext->time_base.num > 1000 && m_decoderContext->time_base.den == 1) {
		m_decoderContext->time_base.den = 1000;
	}

	const auto format = AV_PIX_FMT_RGB24;
	m_size = glm::uvec2(m_decoderContext->width, m_decoderContext->height);
	m_bufferSize = 3u * m_size.x * m_size.y;

	m_swsContext = sws_getContext(
		m_decoderContext->width, m_decoderContext->height, m_decoderContext->pix_fmt,
		m_size.x, m_size.y, format,
		SWS_BICUBIC, nullptr, nullptr, nullptr);

	m_buffer = av_frame_alloc();
	m_buffer->width = m_size.x;
	m_buffer->height = m_size.y;
	m_buffer->format = format;

	const auto size = av_image_get_buffer_size(format,
		m_buffer->width,
		m_buffer->height,
		1);

	m_bufferData.resize(size);

	av_image_fill_arrays(m_buffer->data, m_buffer->linesize, m_bufferData.data(),
		format, m_buffer->width, m_buffer->height, 1);

	m_isInitialized = true;
}

void VideoStream::clear()
{
	if (!m_isInitialized) {
		return;
	}

	m_isInitialized = false;

	std::unique_lock<std::mutex> lockDecoder(m_decoderMutex);
	std::unique_lock<std::mutex> lockReceiver(m_receiverMutex);

	while (!m_frameQueue.empty()) {
		av_frame_free(&m_frameQueue.back());
		m_frameQueue.pop_back();
	}

	m_decoder = nullptr;
	if (m_decoderContext != nullptr) {
		avcodec_free_context(&m_decoderContext);
	}

	if (m_buffer != nullptr) {
		av_frame_free(&m_buffer);
	}

	sws_freeContext(m_swsContext);
	m_swsContext = nullptr;

	m_stream = nullptr;
	m_hasData = false;
	m_decodingId = 0;
	m_hasStarted = false;
}

void VideoStream::receive(AVPacket* packet)
{
	std::unique_lock<std::mutex> receiverLock(m_receiverMutex);

	if (!m_isInitialized) {
		return;
	}

	if (avcodec_send_packet(m_decoderContext, packet) < 0) {
		throw std::runtime_error("Unable to send video packet for decoding");
	}

	AVFrame* frame = av_frame_alloc();

	if (avcodec_receive_frame(m_decoderContext, frame) < 0) {
		av_frame_free(&frame);
	}
	else {
		//printf("Received video frame: %lld\n", frame->pts);

		std::lock_guard<std::mutex> queueLock(m_queueMutex);
		m_frameQueue.push_back(frame);
	}
}

void VideoStream::decode()
{
	std::unique_lock<std::mutex> decoderLock(m_decoderMutex);

	if (!m_isInitialized || m_frameQueue.empty()) {
		return;
	}

	AVFrame* frame = m_frameQueue.front();

	if (frame == nullptr) {
		return;
	}

	if (!m_hasStarted) {
		m_state.restart(m_stream->start_time * av_q2d(m_stream->time_base));
		m_hasStarted = true;
	}

	auto seconds = 0.0;
	if (frame->pkt_dts != AV_NOPTS_VALUE) {
		seconds = frame->best_effort_timestamp * av_q2d(m_stream->time_base);
	}

	if (m_state.getCurrentTime() + m_state.getVideoOffset() < seconds) {
		return;
	}

	{
		std::unique_lock<std::mutex> queueLock(m_queueMutex);
		m_frameQueue.pop_front();
	}

	{
		std::unique_lock<std::mutex> bufferLock(m_bufferMutex);
		sws_scale(m_swsContext, frame->data, frame->linesize, 0, m_decoderContext->height,
			m_buffer->data, m_buffer->linesize);

		m_hasData = true;
		m_decodingId++;
	}

	av_frame_free(&frame);
}

void VideoStream::flush()
{
	if (m_isInitialized) {
		avcodec_send_packet(m_decoderContext, nullptr);		
	}
}

int VideoStream::getIndex() const
{
	if (!m_isInitialized) {
		return -1;
	}

	return m_stream->index;
}

size_t VideoStream::shouldReceive() const
{
	return m_frameQueue.size() < 256;
}

glm::uvec2 VideoStream::getSize() const
{
	return m_size;
}

size_t VideoStream::getBufferSize() const
{
	return m_bufferSize;
}

uint64_t VideoStream::getCurrentDecodingId() const
{
	return m_decodingId;
}

bool VideoStream::writeVideoData(uint8_t* destination, size_t size, size_t decodingId)
{
	std::lock_guard<std::mutex> lock(m_bufferMutex);

	if (!m_hasData || m_decodingId == decodingId) {
		return false;
	}

	std::memcpy(destination, m_buffer->data[0], size);

	return true;
}

bool VideoStream::isInitialized() const
{
	return m_isInitialized;
}
