#include "VideoManager.h"

extern "C" {
#include <libavformat/avformat.h>
}

VideoManager::VideoManager(const ej::Core & core) :
	BaseManager(core)
{
	avformat_network_init();
}

VideoManager::~VideoManager()
{
	avformat_network_deinit();
}

void VideoManager::init()
{

}

void VideoManager::setCurrentVideo(Video::ptr video)
{
}

void VideoManager::receiver()
{
}

void VideoManager::decoder()
{
}
