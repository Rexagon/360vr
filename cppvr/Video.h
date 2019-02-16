#pragma once

#include <list>
#include <mutex>
#include <string>

#include <glm/vec2.hpp>
#include <SFML/System/Clock.hpp>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

#include "PointerDefs.h"
#include "AudioStream.h"

class Video : public ej::PointerDefs<Video>
{
public:
	explicit Video(const std::string& file);
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
	void initVideoStream();
	void initAudioStream();

	bool m_isInitialized;
	std::string m_file;

	sf::Clock m_videoTimer;
	double m_currentVideoTime;
	bool m_videoStarted = false;

	sf::Clock m_audioTimer;
	double m_lastAudioDts;
	double m_lastAudioDelay;
	bool m_audioStarted = false;

	glm::vec2 m_size;

	AVFormatContext* m_formatContext;
	AVPacket m_packet;

	AVCodec* m_videoDecoder;
	AVCodecContext* m_videoDecoderContext;
	SwsContext* m_swsContext;

	AVCodec* m_audioDecoder;
	AVCodecContext* m_audioDecoderContext;
	SwrContext* m_swrContext;

	AVStream* m_videoStream;
	AVStream* m_audioStream;

	std::mutex m_videoBufferMutex;
	bool m_hasVideoData;
	AVFrame* m_videoBuffer;
	uint8_t* m_videoBufferFrameData;

	bool m_hasAudioData;
	std::vector<uint8_t> m_audioBuffer;

	std::mutex m_videoQueueMutex;
	std::list<AVFrame*> m_videoQueue;

	std::mutex m_audioQueueMutex;
	std::list<AVFrame*> m_audioQueue;

	AudioStream m_soundStream;
};