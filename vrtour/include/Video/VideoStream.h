#pragma once

#include <list>
#include <mutex>
#include <vector>

#include <glm/vec2.hpp>
#include <SFML/System/Clock.hpp>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
}

#include "VideoState.h"

namespace app
{
	class VideoStream
	{
	public:
		VideoStream(VideoState& state, AVStream* stream);
		~VideoStream();

		void init();
		void clear();

		void receive(AVPacket* packet);
		void decode();

		void flush();

		bool isInitialized() const;

		int getIndex() const;

		size_t shouldReceive() const;

		glm::uvec2 getSize() const;
		size_t getBufferSize() const;

		uint64_t getCurrentDecodingId() const;
		bool writeVideoData(uint8_t* destination, size_t size, uint64_t decodingId);

	private:
		bool m_isInitialized = false;

		VideoState& m_state;
		AVStream* m_stream;

		std::mutex m_receiverMutex;

		std::mutex m_decoderMutex;
		AVCodec* m_decoder = nullptr;
		AVCodecContext* m_decoderContext = nullptr;
		SwsContext* m_swsContext = nullptr;
		uint64_t m_decodingId = 0;

		std::mutex m_bufferMutex;
		AVFrame* m_buffer = nullptr;
		std::vector<uint8_t> m_bufferData;

		glm::uvec2 m_size{};
		size_t m_bufferSize = 0;
		bool m_hasData = false;

		std::mutex m_queueMutex;
		std::list<AVFrame*> m_frameQueue;

		bool m_hasStarted = false;
	};
}
