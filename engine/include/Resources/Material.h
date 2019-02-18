#pragma once

#include "Core/PointerDefs.h"
#include "Resources/Shader.h"
#include "Managers/ShaderManager.h"

namespace ej
{
	class RenderingManager;

	class Material
	{
	public:
		Material(const Core& core);
		virtual ~Material() = default;

		virtual void bind() = 0;

		Shader* getShader() const;

	protected:
		ShaderManager::ptr m_shaderManager;
		std::shared_ptr<RenderingManager> m_renderingManager;

		Shader::ptr m_shader;
	};
}