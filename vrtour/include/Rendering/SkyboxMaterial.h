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

		void setSkyTexture(ej::Texture* texture);
		ej::Texture* getSkyTexture() const;

		void setNextSkyTexture(ej::Texture* texture);
		ej::Texture* getNextSkyTexture() const;

		void setTransition(float transition);
		float getTransition() const;

	private:
		ej::Texture* m_skyTexture = nullptr;
		ej::Texture* m_nextSkyTexture = nullptr;

		float m_transition = 0.0f;
	};
}
