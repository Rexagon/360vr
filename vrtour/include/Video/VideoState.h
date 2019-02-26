#pragma once

#include <mutex>

#include <SFML/System/Clock.hpp>

class VideoState
{
public:
	void restart(double offset);

	void updateAudioTimings(double dts, double delay);

	double getCurrentTime() const;

	void setVideoOffset(double offset);
	double getVideoOffset() const;

	double getNextAudioDts() const;
	double getLastAudioDts() const;
	double getLastAudioDelay() const;

private:
	std::mutex m_mutex;

	sf::Clock m_clock;
	double m_timeOffset = 0.0;
	double m_videoOffset = 0.0;

	double m_lastAudioDts = 0.0;
	double m_lastAudioDelay = 0.0;
};
