#pragma once

#include "Resources/Shader.h"
#include "Managers/ShaderManager.h"
#include "Rendering/RenderingParameters.h"

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
		explicit Material(const Core& core);

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

		/**
		 * \return Material specific rendering parameters
		 */
		const RenderingParameters& getRenderingParameters() const;

	protected:
		ShaderManager* m_shaderManager = nullptr;
		RenderingManager* m_renderingManager = nullptr;

		RenderingParameters m_renderingParameters;

		Shader* m_shader = nullptr;
	};
}
