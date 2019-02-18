#pragma once

#include <SFML/Audio/SoundStream.hpp>

class Video;

class AudioStream : public sf::SoundStream
{
public:
	AudioStream(Video* video);

	void initialize(unsigned int channelCount, unsigned int sampleRate);

protected:
	bool onGetData(Chunk& data) override;

	void onSeek(sf::Time timeOffset) override;

private:
	Video* m_video;
};