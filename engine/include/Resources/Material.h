#pragma once

#include "Core/PointerDefs.h"
#include "Resources/Shader.h"
#include "Managers/ShaderManager.h"

namespace ej
{
	class RenderingManager;

	/**
	 * \brief Material base class
	 */
	class Material
	{
	public:
		/**
		 * \param core Main core object
		 */
		Material(const Core& core);

		/**
		 * \brief Default destructor
		 */
		virtual ~Material() = default;

		/**
		 * \brief Apply material
		 * 
		 * This is a pure virtual function that has to be implemented
		 * by the derived class to define what should be done before
		 * mesh rendering.
		 */
		virtual void bind() = 0;

		/**
		 * \brief Get main shader
		 * 
		 * Main should be created during initialization.
		 * Material can be used without it, but it is strange.
		 * 
		 * \return Shader
		 */
		Shader* getShader() const;

	protected:
		ShaderManager::ptr m_shaderManager;
		std::shared_ptr<RenderingManager> m_renderingManager;

		Shader::ptr m_shader;
	};
}
