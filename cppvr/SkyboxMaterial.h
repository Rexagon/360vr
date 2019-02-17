#pragma once

#include "Texture.h"
#include "Material.h"

class SkyboxMaterial : public ej::Material, public ej::PointerDefs<SkyboxMaterial>
{
public:
	explicit SkyboxMaterial(const ej::Core& core);

	void bind() override;

	void setSkyTexture(ej::Texture::ptr texture);
	ej::Texture* getTexture() const;

private:
	ej::Texture::ptr m_skyTexture;
};
