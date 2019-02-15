#include "AudioStream.h"



void AudioStream::initialize(unsigned channelCount, unsigned sampleRate)
{
	SoundStream::initialize(channelCount, sampleRate);
}

bool AudioStream::onGetData(Chunk& data)
{
	return true;
}

void AudioStream::onSeek(sf::Time timeOffset)
{
}
