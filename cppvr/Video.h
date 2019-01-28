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
}

#include "PointerDefs.h"

class Video : public ej::PointerDefs<Video>
{
public:
	explicit Video(const std::string& file);
	~Video();

	void init();

	void receive();
	bool shouldReceive() const;
	void flush() const;

	void decode();

	void play();
	void pause();
	bool isPlaying() const;

	glm::vec2 getSize() const;
	bool hasData() const;

	bool writeData(uint8_t* destination, size_t size);

private:
	bool m_isInitialized;
	std::string m_file;

	bool m_isPlaying;

	sf::Clock m_timer;
	float m_currentVideoTime;

	glm::vec2 m_size;

	AVCodec* m_decoder;
	AVCodecContext* m_decoderContext;
	AVFormatContext* m_formatContext;
	SwsContext* m_swsContext;

	int m_videoStreamIndex;

	std::mutex m_bufferMutex;
	bool m_hasData;
	AVFrame* m_buffer;
	uint8_t* m_bufferFrameData;
	AVPacket m_packet;

	std::mutex m_frameQueueMutex;
	std::list<AVFrame*> m_frameQueue;
};