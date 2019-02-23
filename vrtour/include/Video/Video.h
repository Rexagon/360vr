#pragma once

#include <list>
#include <mutex>
#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <SFML/System/Clock.hpp>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

#include <Core/PointerDefs.h>

#include "AudioStream.h"
#include "Managers/VideoManager.h"

class Video : public ej::PointerDefs<Video>
{
public:
	explicit Video(const ej::Core& core, const std::string& file);
	~Video();

	void init();

	void receive();
	bool shouldReceive() const;
	void flush() const;

	void decodeVideo();
	void decodeAudio();

	glm::vec2 getSize() const;
	bool hasVideoData() const;

	bool writeVideoData(uint8_t* destination, size_t size);
	bool writeAudioData(const int16_t** destination, size_t& size);

private:
	void initializationTask();
	void initVideoStream();
	void initAudioStream();

	VideoManager::ptr m_videoManager;

	std::string m_file;
	bool m_isInitialized = false;

	sf::Clock m_videoTimer;
	double m_currentVideoTime = 0.0;
	bool m_videoStarted = false;

	double m_lastAudioDts = 0.0;
	double m_lastAudioDelay = 0.0;
	bool m_audioStarted = false;

	glm::vec2 m_size{};

	AVFormatContext* m_formatContext = nullptr;
	AVPacket m_packet{};

	AVCodec* m_videoDecoder = nullptr;
	AVCodecContext* m_videoDecoderContext = nullptr;
	SwsContext* m_swsContext = nullptr;

	AVCodec* m_audioDecoder = nullptr;
	AVCodecContext* m_audioDecoderContext = nullptr;
	SwrContext* m_swrContext = nullptr;

	AVStream* m_videoStream = nullptr;
	AVStream* m_audioStream = nullptr;

	std::mutex m_videoBufferMutex;
	bool m_hasVideoData = false;
	AVFrame* m_videoBuffer = nullptr;
	uint8_t* m_videoBufferFrameData = nullptr;

	bool m_hasAudioData = false;
	std::vector<uint8_t> m_audioBuffer;

	std::mutex m_videoQueueMutex;
	std::list<AVFrame*> m_videoQueue;

	std::mutex m_audioQueueMutex;
	std::list<AVFrame*> m_audioQueue;

	AudioStream m_soundStream{this};
};