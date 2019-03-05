#include "Resources/Material.h"

#include "Managers/RenderingManager.h"

ej::Material::Material(const Core& core)
{
	m_shaderManager = core.get<ShaderManager>();
	m_renderingManager = core.get<RenderingManager>();
}

ej::Shader* ej::Material::getShader() const
{
	return m_shader;
}
