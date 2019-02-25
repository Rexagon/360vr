#include "Video/VideoState.h"

void VideoState::restart(double offset)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	m_clock.restart();
	m_timeOffset = offset;
}

void VideoState::updateAudioTimings(double dts, double delay)
{
	m_lastAudioDts = dts;
	m_lastAudioDelay = delay;
}

double VideoState::getCurrentTime() const
{
	const auto dt = static_cast<double>(m_clock.getElapsedTime().asSeconds());
	return m_timeOffset + dt;
}

double VideoState::getNextAudioDts() const
{
	return m_lastAudioDts + m_lastAudioDelay;
}

double VideoState::getLastAudioDts() const
{
	return m_lastAudioDts;
}

double VideoState::getLastAudioDelay() const
{
	return m_lastAudioDelay;
}
