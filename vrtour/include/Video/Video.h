#pragma once

#include <mutex>
#include <string>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include "VideoState.h"
#include "AudioStream.h"
#include "VideoStream.h"
#include "Managers/VideoManager.h"

class Video final
{
public:
	explicit Video(const ej::Core& core, const std::string& file);
	~Video();

	void init();

	VideoStream* getVideoStream() const;
	AudioStream* getAudioStream() const;

	bool isInitialized() const;

private:
	/**
	 * \brief Connection timeout in milliseconds
	 */
	static const int32_t CONNECTION_TIMEOUT = 10000;

	void initializationTask();
	void receivingTask();
	void decodingTask();

	void receive();

	void clear();

	static int connectionCallback(void* data);
	static int interruptionCallback(void* data);

	VideoManager* m_videoManager{nullptr};

	std::string m_file;

	std::mutex m_receiverMutex;
	AVFormatContext* m_formatContext = nullptr;
	AVPacket m_packet{};

	std::unique_ptr<VideoStream> m_videoStream;
	std::unique_ptr<AudioStream> m_audioStream;

	VideoState m_state;

	bool m_isInitialized = false;
};