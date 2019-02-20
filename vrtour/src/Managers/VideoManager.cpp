#include "Managers/VideoManager.h"

extern "C" {
#include <libavformat/avformat.h>
}

VideoManager::VideoManager(const ej::Core & core) :
	BaseManager(core),
	m_isInitialized(false), m_isReceiving(false), m_isDecodingVideo(false)
{
}

VideoManager::~VideoManager()
{
	if (!m_isInitialized) {
		return;
	}

	m_isReceiving = false;
	m_isDecodingVideo = false;

	m_receiverThread->join();
	m_videoDecoderThread->join();

	avformat_network_deinit();
}

void VideoManager::init()
{
	if (m_isInitialized) {
		return;
	}

	avformat_network_init();

	m_receiverThread = std::make_unique<std::thread>([this]() {
		m_isReceiving = true;
		while (m_isReceiving) {
			auto now = std::chrono::high_resolution_clock::now();

			if (m_currentVideo != nullptr && m_currentVideo->shouldReceive()) {
				m_currentVideo->receive();
			}
			
			std::this_thread::sleep_until(now + std::chrono::milliseconds(1));
		}
	});

	m_videoDecoderThread = std::make_unique<std::thread>([this]() {
		m_isDecodingVideo = true;
		while (m_isDecodingVideo) {
			auto now = std::chrono::high_resolution_clock::now();

			if (m_currentVideo != nullptr) {
				m_currentVideo->decodeVideo();
			}

			std::this_thread::sleep_until(now + std::chrono::milliseconds(1));
		}
	});

	m_isInitialized = true;
}

void VideoManager::setCurrentVideo(Video::ptr video)
{
	std::unique_lock<std::mutex> lock(m_currentVideoMutex);
	m_currentVideo = video;
}

bool VideoManager::isInitialized() const
{
	return m_isInitialized;
}