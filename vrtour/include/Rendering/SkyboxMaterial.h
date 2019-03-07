#pragma once

#include <Resources/Texture.h>
#include <Resources/Material.h>

namespace app
{
	class SkyBoxMaterial final : public ej::Material
	{
	public:
		explicit SkyBoxMaterial(const ej::Core& core);

		void bind() override;

		void update(float dt);

		void startTransition();
		void finishTransition();
		void pauseTransition();
		void resetTransition();

		void setSkyTexture(ej::Texture* texture);
		ej::Texture* getSkyTexture() const;

		void setNextSkyTexture(ej::Texture* texture);
		ej::Texture* getNextSkyTexture() const;

		float getTransition() const;

		void setTransitionSpeed(float speed);
		float getTransitionSpeed() const;

	private:
		ej::Texture* m_skyTexture = nullptr;
		ej::Texture* m_nextSkyTexture = nullptr;

		float m_transition = 0.0f;
		float m_transitionSpeed = 1.0f;

		bool m_isPaused = true;
	};
}
