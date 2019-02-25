#pragma once

#include <mutex>

#include <glm/vec2.hpp>
#include <SFML/System/Clock.hpp>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
}

class VideoStream
{
public:
	VideoStream(AVStream* stream);
	~VideoStream();

	void init();
	void clear();

	void decode();

	glm::uvec2 getSize() const;
	size_t getBufferSize() const;

	uint64_t getCurrentDecodingId() const;
	bool writeVideoData(uint8_t* destination, size_t size, uint64_t decodingId);

	double getCurrentTime() const;

	bool isInitialized() const;

private:
	AVStream* m_stream;

	std::mutex m_decoderMutex;
	AVCodec* m_decoder = nullptr;
	AVCodecContext* m_decoderContext = nullptr;
	SwsContext* m_swsContext = nullptr;
	uint64_t m_decodingId = 0;

	std::mutex m_bufferMutex;
	AVFrame* m_buffer = nullptr;

	glm::uvec2 m_size{};
	size_t m_bufferSize = 0;

	std::mutex m_queueMutex;
	std::list<AVFrame*> m_frameQueue;

	sf::Clock m_clock;
	double m_time = 0.0;
	bool m_hasStarted = false;

	bool m_isInitialized = false;
};