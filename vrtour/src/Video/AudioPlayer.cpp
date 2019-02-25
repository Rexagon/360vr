#include "Video/AudioPlayer.h"

#include "Video/Video.h"

AudioPlayer::AudioPlayer(unsigned channelCount, unsigned sampleRate, const Provider& provider) :
	m_provider(provider)
{
	initialize(channelCount, sampleRate);
}

bool AudioPlayer::onGetData(Chunk& data)
{
	return m_provider(&data.samples, data.sampleCount);
}

void AudioPlayer::onSeek(sf::Time timeOffset)
{
}
