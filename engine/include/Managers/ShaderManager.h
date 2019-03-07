#pragma once

#include <memory>

#include "Resources/Shader.h"
#include "Core/ResourceManager.h"

namespace ej
{
	class FileManager;

	struct ShaderSource
	{
		enum Type { None, File, String };

		ShaderSource() :
			type(None)
		{}

		ShaderSource(const Type type, const std::string& source) :
			type(type), source(source)
		{}

		Type type;
		std::string source;
	};

	namespace details {
		struct ShaderFactoryData final
		{
			std::string name;
			ShaderSource vertexShaderSource;
			ShaderSource geometryShaderSource;
			ShaderSource fragmentShaderSource;
		};
	}

	/**
	 * \brief Manage shaders
	 */
	class ShaderManager final : public ResourceManager<ShaderManager, Shader, details::ShaderFactoryData>
	{
	public:
		struct FromFile final : ShaderSource
		{
			explicit FromFile(const std::string& source) : ShaderSource(File, source) {}
		};

		struct FromString final : ShaderSource
		{
			explicit FromString(const std::string& source) : ShaderSource(String, source) {}
		};

		/**
		 * \param core Owner of this manager
		 */
		explicit ShaderManager(const Core& core);

		/**
		 * \brief Register shader loader
		 * \param name Resource name
		 * \param vertexShaderSource Vertex shader source
		 * \param fragmentShaderSource Fragment shader source. None by default
		 * \param geometryShaderSource Geometry shader source. None by default
		 * \return this
		 */
		ShaderManager* bind(const std::string& name, const ShaderSource& vertexShaderSource, 
			const ShaderSource& fragmentShaderSource = ShaderSource(), 
			const ShaderSource& geometryShaderSource = ShaderSource());

	private:
		/**
		 * \brief Load shader
		 * 
		 * \throw std::runtime_error if unable to load.
		 * 
		 * \param factoryData Shader factory data
		 * \return Shader
		 */
		std::unique_ptr<Shader> load(const FactoryDataType& factoryData) override;

		FileManager* m_fileManager = nullptr;
	};
}