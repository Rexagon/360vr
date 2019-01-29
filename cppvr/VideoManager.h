#pragma once

#include <thread>

#include "Video.h"
#include "BaseManager.h"

class VideoManager : public ej::BaseManager, public ej::PointerDefs<VideoManager>
{
public:
	explicit VideoManager(const ej::Core& core);
	~VideoManager();

	void init();

	void setCurrentVideo(Video::ptr video);

	bool isInitialized() const;

private:
	void receiver();
	void decoder();

	bool m_isInitialized;

	std::mutex m_currentVideoMutex;
	Video::ptr m_currentVideo;

	std::unique_ptr<std::thread> m_receiverThread;
	bool m_isReceiving;

	std::unique_ptr<std::thread> m_decoderThread;
	bool m_isDecoding;
};
