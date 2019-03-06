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

	private:
		ej::Texture* m_skyTexture = nullptr;
	};
}
