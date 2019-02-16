#include "AudioStream.h"

#include "Video.h"

AudioStream::AudioStream(Video* video) :
	m_video(video)
{
}

void AudioStream::initialize(unsigned channelCount, unsigned sampleRate)
{
	SoundStream::initialize(channelCount, sampleRate);
}

bool AudioStream::onGetData(Chunk& data)
{
	if (m_video == nullptr) {
		return false;
	}

	return m_video->writeAudioData(&data.samples, data.sampleCount);
}

void AudioStream::onSeek(sf::Time timeOffset)
{
}
