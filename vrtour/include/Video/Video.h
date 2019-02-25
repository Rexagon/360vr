#pragma once

#include <mutex>
#include <string>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <Core/PointerDefs.h>

#include "VideoState.h"
#include "AudioStream.h"
#include "VideoStream.h"
#include "Managers/VideoManager.h"

class Video : public ej::PointerDefs<Video>
{
public:
	explicit Video(const ej::Core& core, const std::string& file);
	~Video();

	void init();

	VideoStream* getVideoStream() const;
	AudioStream* getAudioStream() const;

	bool isInitialized() const;

private:
	void initializationTask();
	void receivingTask();
	void decodingTask();

	void receive();

	void clear();

	VideoManager::ptr m_videoManager;

	std::string m_file;

	std::mutex m_receiverMutex;
	AVFormatContext* m_formatContext = nullptr;
	AVPacket m_packet{};

	std::shared_ptr<VideoStream> m_videoStream;
	std::shared_ptr<AudioStream> m_audioStream;

	VideoState m_state;

	bool m_isInitialized = false;
};