#pragma once

#include <mutex>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

#include "AudioPlayer.h"
#include "VideoState.h"

class AudioStream
{
public:
	AudioStream(VideoState& state, AVStream* stream);
	~AudioStream();

	void init();
	void clear();

	void receive(AVPacket* packet);
	void decode();

	void flush();

	int getIndex() const;

	size_t shouldReceive() const;

	void setVolume(float volume);

	uint64_t getCurrentDecodingId() const;

	bool isInitialized() const;

private:
	VideoState& m_state;
	AVStream* m_stream;

	std::mutex m_receiverMutex;

	std::mutex m_decoderMutex;
	AVCodec* m_decoder = nullptr;
	AVCodecContext* m_decoderContext = nullptr;
	SwrContext* m_swrContext = nullptr;
	uint64_t m_decodingId = 0;

	std::mutex m_bufferMutex;
	std::vector<uint8_t> m_buffer;
	size_t m_sampleCount = 0;
	bool m_hasData = false;

	std::shared_ptr<AudioPlayer> m_audioPlayer;

	std::mutex m_queueMutex;
	std::list<AVFrame*> m_frameQueue;

	bool m_hasStarted = false;

	bool m_isInitialized = false;
};