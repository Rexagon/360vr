#pragma once

#include <functional>
#include <SFML/Audio/SoundStream.hpp>

namespace app
{
	class AudioPlayer final : public sf::SoundStream
	{
	public:
		using Provider = std::function<bool(int16_t const**, size_t&)>;

		AudioPlayer(unsigned channelCount, unsigned sampleRate, const Provider& provider);

	protected:
		bool onGetData(Chunk& data) override;

		void onSeek(sf::Time timeOffset) override;

	private:
		Provider m_provider;
	};
}
