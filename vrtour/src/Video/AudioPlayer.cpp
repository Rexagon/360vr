#include "Video/AudioPlayer.h"

#include "Video/Video.h"

app::AudioPlayer::AudioPlayer(unsigned channelCount, unsigned sampleRate, const Provider& provider) :
	m_provider(provider)
{
	initialize(channelCount, sampleRate);
}

bool app::AudioPlayer::onGetData(Chunk& data)
{
	return m_provider(&data.samples, data.sampleCount);
}

void app::AudioPlayer::onSeek(sf::Time timeOffset)
{
	//Do nothing
}
