#include "Managers/VideoManager.h"

#include <sstream>
#include <functional>
#include <asio/post.hpp>

extern "C" {
#include <libavformat/avformat.h>
}

app::VideoManager::VideoManager(const ej::Core & core) :
	BaseManager(core)
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

app::VideoManager::~VideoManager()
{
	if (!m_isInitialized) {
		return;
	}

	m_ioService->stop();
	m_threadGroup->join();

	avformat_network_deinit();
}

void app::VideoManager::init()
{
	if (m_isInitialized) {
		return;
	}

	avformat_network_init();

	m_ioService = std::make_unique<asio::io_service>();
	m_ioServiceWork = std::make_unique<asio::io_service::work>(*m_ioService);

	printf("Starting %zu worker threads...\n", m_threadCount);

	m_threadGroup = std::make_unique<asio::detail::thread_group>();
	m_threadGroup->create_threads(std::bind(&VideoManager::worker, this), m_threadCount);

	m_isInitialized = true;
}

bool app::VideoManager::isInitialized() const
{
	return m_isInitialized;
}

asio::io_service* app::VideoManager::getService() const
{
	return m_ioService.get();
}

asio::io_service::strand* app::VideoManager::getStrand() const
{
	return m_strand.get();
}

void app::VideoManager::worker()
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
