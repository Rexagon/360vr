#include "VideoStream.h"

extern "C" {
#include <libavutil/imgutils.h>
}

VideoStream::VideoStream() :
	m_isInitialized(false),
	m_decoder(nullptr), m_swsContext(nullptr),
	m_decoderContext(nullptr), m_formatContext(nullptr), 
	m_videoStreamIndex(0),
	m_frame(nullptr), m_buffer(nullptr), m_packet(), m_size(),
	m_hasData(false)
{
}

VideoStream::~VideoStream()
{
	if (m_isInitialized) {
		av_frame_free(&m_frame);
		av_frame_free(&m_buffer);
		av_free(m_bufferFrameData);

		avformat_close_input(&m_formatContext);
		avcodec_free_context(&m_decoderContext);

		avformat_network_deinit();
		sws_freeContext(m_swsContext);
	}
}

bool VideoStream::init(const std::string& url)
{
	if (m_isInitialized) {
		return true;
	}

	m_url = url;

	// Connect to stream
	avformat_network_init();

	if (avformat_open_input(&m_formatContext, m_url.data(), nullptr, nullptr) < 0) {
		throw std::runtime_error("Could not open input file\n");
	}
	if (avformat_find_stream_info(m_formatContext, nullptr) < 0) {
		throw std::runtime_error("Failed to retrieve input stream information\n");
	}

	printf("Successfully connected to stream %s\n", m_url.data());

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
		AV_PIX_FMT_RGB24, SWS_BICUBIC, nullptr,	nullptr, nullptr);

	// Create buffers
	m_size = glm::vec2(m_decoderContext->width, m_decoderContext->height);

	av_init_packet(&m_packet);
	m_frame = av_frame_alloc();

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
	return true;
}

void VideoStream::startReceiving(const bool& receiving)
{
	while (receiving && av_read_frame(m_formatContext, &m_packet) >= 0) {
		if (m_packet.stream_index == m_videoStreamIndex) {

			const auto ret = avcodec_send_packet(m_decoderContext, &m_packet);
			if (ret >= 0) {

				while (avcodec_receive_frame(m_decoderContext, m_frame) >= 0)
				{
					// printf("[Frame] t: %lld\tw: %d\th: %d\n", m_frame->best_effort_timestamp, m_frame->width, m_frame->height);

					std::lock_guard<std::mutex> lock(m_dataMutex);
					sws_scale(m_swsContext, m_frame->data, m_frame->linesize, 0, m_decoderContext->height, 
						m_buffer->data, m_buffer->linesize);
					m_hasData = true;
				}
			}

		}

		av_packet_unref(&m_packet);
	}
}

bool VideoStream::isInitialized() const
{
	return m_isInitialized;
}

glm::vec2 VideoStream::getSize() const
{
	return m_size;
}

bool VideoStream::hasData() const
{
	return m_hasData;
}

bool VideoStream::sendCurrentData(uint8_t* dst, size_t size)
{
	std::lock_guard<std::mutex> lock(m_dataMutex);

	if (m_buffer == nullptr) {
		return false;
	}

	std::memcpy(dst, m_buffer->data[0], size);

	m_hasData = false;
	return true;
}
