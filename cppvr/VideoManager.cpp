#include "VideoManager.h"

extern "C" {
#include <libavformat/avformat.h>
}

VideoManager::VideoManager(const ej::Core & core) :
	BaseManager(core),
	m_isInitialized(false), m_isReceiving(false), m_isDecoding(false)
{
}

VideoManager::~VideoManager()
{
	if (!m_isInitialized) {
		return;
	}

	m_isReceiving = false;
	m_isDecoding = false;

	m_receiverThread->join();
	m_decoderThread->join();

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

			receiver();
			
			std::this_thread::sleep_until(now + std::chrono::milliseconds(1));
		}
	});

	m_decoderThread = std::make_unique<std::thread>([this]() {
		m_isDecoding = true;
		while (m_isDecoding) {
			auto now = std::chrono::high_resolution_clock::now();

			decoder();

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

void VideoManager::receiver()
{
	if (m_currentVideo == nullptr || !m_currentVideo->shouldReceive()) {
		return;
	}

	m_currentVideo->receive();
}

void VideoManager::decoder()
{
	if (m_currentVideo == nullptr) {
		return;
	}

	m_currentVideo->decode();
}
