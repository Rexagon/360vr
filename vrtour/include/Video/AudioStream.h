#pragma once

#include <mutex>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

#include "AudioPlayer.h"

class AudioStream
{
public:
	AudioStream(AVStream* stream);
	~AudioStream();

	void init();
	void clear();

	void decode();

	void setVolume(float volume);

	uint64_t getCurrentDecodingId() const;

	bool isInitialized();

private:
	AVStream* m_stream;

	std::mutex m_decoderMutex;
	AVCodec* m_decoder = nullptr;
	AVCodecContext* m_decoderContext = nullptr;
	SwrContext* m_swrContext = nullptr;
	uint64_t m_decodingId = 0;

	std::mutex m_bufferMutex;
	std::vector<uint8_t> m_buffer;

	std::shared_ptr<AudioPlayer> m_audioPlayer;

	std::mutex m_queueMutex;
	std::list<AVFrame*> m_frameQueue;

	double m_lastAudioDts = 0.0;
	double m_lastAudioDelay = 0.0;
	bool m_hasStarted = false;

	bool m_isInitialized = false;
};