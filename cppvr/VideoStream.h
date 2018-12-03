#pragma once

#include <mutex>
#include <atomic>

#include <glm/vec2.hpp>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
}

class VideoStream
{
public:
	VideoStream();
	~VideoStream();

	bool init(const std::string& url);
	void startReceiving(std::atomic_bool& receiving);

	bool isInitialized() const;

	glm::vec2 getSize() const;

	bool hasData() const;
	bool sendCurrentData(uint8_t* dst, size_t size);

private:
	bool m_isInitialized;
	std::string m_url;

	AVCodec* m_decoder;
	SwsContext* m_swsContext;
	AVCodecContext* m_decoderContext;
	AVFormatContext* m_formatContext;
	int m_videoStreamIndex;

	AVFrame* m_frame;
	AVFrame* m_buffer;
	uint8_t* m_bufferFrameData;
	AVPacket m_packet;

	glm::vec2 m_size;

	bool m_hasData;
	std::mutex m_dataMutex;
};