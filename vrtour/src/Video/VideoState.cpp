#include "Video/VideoState.h"

void app::VideoState::restart(double offset)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	m_clock.restart();
	m_timeOffset = offset;
	m_videoOffset = 0.0;
}

void app::VideoState::updateAudioTimings(double dts, double delay)
{
	m_lastAudioDts = dts;
	m_lastAudioDelay = delay;
}

double app::VideoState::getCurrentTime() const
{
	const auto dt = static_cast<double>(m_clock.getElapsedTime().asSeconds());
	return m_timeOffset + dt;
}

void app::VideoState::setVideoOffset(const double offset)
{
	m_videoOffset = offset;
}

double app::VideoState::getVideoOffset() const
{
	return m_videoOffset;
}

double app::VideoState::getNextAudioDts() const
{
	return m_lastAudioDts + m_lastAudioDelay;
}

double app::VideoState::getLastAudioDts() const
{
	return m_lastAudioDts;
}

double app::VideoState::getLastAudioDelay() const
{
	return m_lastAudioDelay;
}
