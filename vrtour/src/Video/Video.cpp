#include "Video/Video.h"

#include <asio/post.hpp>

extern "C" {
#include <libavutil/opt.h>
}

#include <Core/Core.h>

static int interruptCallback(void* video)
{
	return static_cast<int>(video == nullptr || !reinterpret_cast<Video*>(video)->isInitialized());
}

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

VideoStream* Video::getVideoStream() const
{
	return m_videoStream.get();
}

AudioStream* Video::getAudioStream() const
{
	return m_audioStream.get();
}

bool Video::isInitialized() const
{
	return m_isInitialized;
}

void Video::initializationTask()
{
	// Connect to data stream

	// AVDictionary* formatOptions = nullptr;
	// av_dict_set(&formatOptions, "timeout", "60", 0);

	if (avformat_open_input(&m_formatContext, m_file.data(), nullptr, nullptr) < 0) {
		throw std::runtime_error("Could not open input file\n");
	}

	if (avformat_find_stream_info(m_formatContext, nullptr) < 0) {
		clear();
		throw std::runtime_error("Failed to retrieve input stream information\n");
	}

	printf("Successfully opened data stream %s\n", m_file.data());

	AVStream* video = nullptr;
	AVStream* audio = nullptr;
	for (unsigned int i = 0; i < m_formatContext->nb_streams; i++) {
		const auto codecType = m_formatContext->streams[i]->codecpar->codec_type;
		printf("Stream: %d, Codec: %d\n", i, codecType);
		if (codecType == AVMEDIA_TYPE_VIDEO) {
			video = m_formatContext->streams[i];
		}
		else if (codecType == AVMEDIA_TYPE_AUDIO) {
			audio = m_formatContext->streams[i];
		}
	}

	m_formatContext->interrupt_callback.opaque = static_cast<void*>(this);
	m_formatContext->interrupt_callback.callback = interruptCallback;

	m_videoStream = std::make_shared<VideoStream>(m_state, video);
	m_videoStream->init();

	m_audioStream = std::make_shared<AudioStream>(m_state, audio);
	m_audioStream->init();

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

	m_videoStream->decode();

	post(*m_videoManager->getService(), std::bind(&Video::decodingTask, this));
}

void Video::receive()
{
	std::unique_lock<std::mutex> lockReceiver(m_receiverMutex);

	if (!m_isInitialized || !m_videoStream->shouldReceive() && !m_audioStream->shouldReceive()) {
		return;
	}

	if (av_read_frame(m_formatContext, &m_packet) < 0) {
		printf("Error: Unable to read frame\n");
		return;
	}

	if (m_packet.stream_index == m_videoStream->getIndex()) {
		m_videoStream->receive(&m_packet);
	}
	else if (m_packet.stream_index == m_audioStream->getIndex()) {
		m_audioStream->receive(&m_packet);
	}

	av_packet_unref(&m_packet);
}

void Video::clear()
{
	m_isInitialized = false;

	std::unique_lock<std::mutex> lockReceiver(m_receiverMutex);
	m_videoStream.reset();
	m_audioStream.reset();

	if (m_formatContext != nullptr) {
		avformat_close_input(&m_formatContext);
	}
}
