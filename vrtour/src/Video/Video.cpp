#include "Video/Video.h"

#include <asio/post.hpp>

extern "C" {
#include <libavutil/opt.h>
}

#include <Core/Core.h>

app::Video::Video(const ej::Core& core, const std::string& file):
	m_file(file)
{
	m_videoManager = core.get<VideoManager>();
}

app::Video::~Video()
{
	clear();
}

void app::Video::init()
{
	if (m_isInitialized) {
		return;
	}

	// Post initialization to video manager
	post(*m_videoManager->getService(), std::bind(&Video::initializationTask, this));
}

app::VideoStream* app::Video::getVideoStream() const
{
	return m_videoStream.get();
}

app::AudioStream* app::Video::getAudioStream() const
{
	return m_audioStream.get();
}

bool app::Video::isInitialized() const
{
	return m_isInitialized;
}

void app::Video::initializationTask()
{
	std::unique_lock<std::mutex> lockReceiver(m_receiverMutex);

	m_formatContext = avformat_alloc_context();

	// Create connection callback
	m_connectionTimer.restart();
	m_callbackState = CallbackState::Connection;
	m_formatContext->interrupt_callback.opaque = static_cast<void*>(this);
	m_formatContext->interrupt_callback.callback = &Video::interruptionCallback;
	
	// Connect to data stream
	if (avformat_open_input(&m_formatContext, m_file.data(), nullptr, nullptr) < 0) {
		throw std::runtime_error("Could not open input file " + m_file + "\n");
	}

	// Change callback
	m_shouldStop = false;
	m_callbackState = CallbackState::Interruption;

	// Retrieve all stream information
	if (avformat_find_stream_info(m_formatContext, nullptr) < 0) {
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

	m_videoStream = std::make_unique<VideoStream>(m_state, video);
	m_videoStream->init();

	m_audioStream = std::make_unique<AudioStream>(m_state, audio);
	m_audioStream->init();

	m_isInitialized = true;

	post(*m_videoManager->getService(), std::bind(&Video::receivingTask, this));
	post(*m_videoManager->getService(), std::bind(&Video::decodingTask, this));
}

void app::Video::receivingTask()
{
	if (!m_isInitialized) {
		return;
	}

	receive();

	post(*m_videoManager->getService(), std::bind(&Video::receivingTask, this));
}

void app::Video::decodingTask()
{
	if (!m_isInitialized) {
		return;
	}

	m_videoStream->decode();

	post(*m_videoManager->getService(), std::bind(&Video::decodingTask, this));
}

void app::Video::receive()
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
		//m_audioStream->receive(&m_packet);
	}

	av_packet_unref(&m_packet);
}

void app::Video::clear()
{
	m_isInitialized = false;
	m_shouldStop = true;

	std::unique_lock<std::mutex> lockReceiver(m_receiverMutex);
	m_videoStream.reset();
	m_audioStream.reset();

	avformat_close_input(&m_formatContext);
}

int app::Video::interruptionCallback(void* data)
{
	if (data == nullptr) {
		return 1;
	}

	const auto* video = reinterpret_cast<Video*>(data);

	switch (video->m_callbackState) {
	case CallbackState::Connection:
		return video->m_connectionTimer.getElapsedTime().asMilliseconds() > CONNECTION_TIMEOUT || video->m_shouldStop;

	case CallbackState::Interruption:
		return video->m_shouldStop;

	default: 
		return 1;
	}
}
