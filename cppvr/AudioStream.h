#pragma once

#include <SFML/Audio/SoundStream.hpp>

class AudioStream : public sf::SoundStream
{
public:
	void initialize(unsigned int channelCount, unsigned int sampleRate);

protected:
	bool onGetData(Chunk& data) override;

	void onSeek(sf::Time timeOffset) override;
};