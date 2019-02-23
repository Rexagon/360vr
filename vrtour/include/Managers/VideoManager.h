#pragma once

#include <mutex>
#include <thread>

#include <Core/BaseManager.h>
#include <asio/io_service.hpp>

class VideoManager : public ej::BaseManager, public ej::PointerDefs<VideoManager>
{
public:
	explicit VideoManager(const ej::Core& core);
	~VideoManager();

	void init();

	bool isInitialized() const;

	asio::io_service* getService() const;
	asio::io_service::strand* getStrand() const;

private:
	void worker();

	bool m_isInitialized;

	size_t m_threadCount;

	std::mutex m_workerThreadMutex;
	std::shared_ptr<asio::io_service> m_ioService;
	std::shared_ptr<asio::io_service::strand> m_strand;
	std::shared_ptr<asio::io_service::work> m_ioServiceWork;
	std::shared_ptr<asio::detail::thread_group> m_threadGroup;
};
