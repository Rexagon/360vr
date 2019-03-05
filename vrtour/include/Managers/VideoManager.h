#pragma once

#include <mutex>

#include <asio/strand.hpp>
#include <asio/io_service.hpp>
#include <Core/BaseManager.h>

class VideoManager final : public ej::BaseManager
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

	bool m_isInitialized{ false };

	size_t m_threadCount{ 1 };

	std::mutex m_workerThreadMutex;
	std::unique_ptr<asio::io_service> m_ioService;
	std::unique_ptr<asio::io_service::strand> m_strand;
	std::unique_ptr<asio::io_service::work> m_ioServiceWork;
	std::unique_ptr<asio::detail::thread_group> m_threadGroup;
};
