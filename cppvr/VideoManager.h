#pragma once

#include "Video.h"
#include "BaseManager.h"

class VideoManager : public ej::BaseManager
{
public:
	explicit VideoManager(const ej::Core& core);
	~VideoManager();

	void init();

	void setCurrentVideo(Video::ptr video);

private:
	void receiver();
	void decoder();

	std::mutex m_currentVideoMutex;
	Video::ptr m_currentVideo;
};