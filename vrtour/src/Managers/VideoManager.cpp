#include "Managers/VideoManager.h"

#include <sstream>
#include <asio/post.hpp>

extern "C" {
#include <libavformat/avformat.h>
}

VideoManager::VideoManager(const ej::Core & core) :
	BaseManager(core),
	m_isInitialized(false)
{
	m_threadCount = std::thread::hardware_concurrency();
	if (m_threadCount < 4) {
		printf("Not enough threads\n");
		m_threadCount = 2;
	}
	else {
		m_threadCount -= 2;
	}
}

VideoManager::~VideoManager()
{
	if (!m_isInitialized) {
		return;
	}

	m_ioService->stop();
	m_threadGroup->join();

	avformat_network_deinit();
}

void VideoManager::init()
{
	if (m_isInitialized) {
		return;
	}

	avformat_network_init();

	m_ioService = std::make_shared<asio::io_service>();
	m_ioServiceWork = std::make_shared<asio::io_service::work>(*m_ioService);

	printf("Starting %zu worker threads...\n", m_threadCount);

	m_threadGroup = std::make_shared<asio::detail::thread_group>();
	m_threadGroup->create_threads(std::bind(&VideoManager::worker, this), m_threadCount);

	m_isInitialized = true;
}

bool VideoManager::isInitialized() const
{
	return m_isInitialized;
}

asio::io_service* VideoManager::getService() const
{
	return m_ioService.get();
}

asio::io_service::strand* VideoManager::getStrand() const
{
	return m_strand.get();
}

void VideoManager::worker()
{
	std::string threadId;
	{
		std::stringstream ss;
		ss << std::this_thread::get_id();
		threadId = ss.str();
	}

	m_workerThreadMutex.lock();
	printf("[%s] Worker thread started\n", threadId.c_str());
	m_workerThreadMutex.unlock();

	while(true) {
		try {
			asio::error_code errorCode;
			m_ioService->run(errorCode);

			if (errorCode) {
				m_workerThreadMutex.lock();
				printf("[%s] Fatal error: %s\n", threadId.c_str(), errorCode.message().c_str());
				m_workerThreadMutex.unlock();
			}
			break;
		}
		catch(std::exception& e) {
			m_workerThreadMutex.lock();
			printf("[%s] Exception: %s\n", threadId.c_str(), e.what());
			m_workerThreadMutex.unlock();
		}
	}

	m_workerThreadMutex.lock();
	printf("[%s] Worker thread stopped\n", threadId.c_str());
	m_workerThreadMutex.unlock();
}
