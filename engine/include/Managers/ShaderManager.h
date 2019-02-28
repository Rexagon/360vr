#pragma once

#include <memory>
#include <unordered_map>

#include "Resources/Shader.h"
#include "Core/ResourceManager.h"

namespace ej
{
	class FileManager;

	/**
	 * \brief Manage shaders
	 */
	class ShaderManager final : public ResourceManager<Shader>, public PointerDefs<ShaderManager>
	{
		struct ShaderSource
		{
			enum Type { None, File, String };

			ShaderSource() :
				type(None)
			{}

			ShaderSource(Type type, const std::string& source) :
				type(type), source(source)
			{}

			Type type;
			std::string source;
		};

		struct FactoryData
		{
			FactoryData(const std::string& name, const ShaderSource& vertexShaderSource,
				const ShaderSource& geometryShaderSource, const ShaderSource& fragmentShaderSource) :
				name(name),	vertexShaderSource(vertexShaderSource), geometryShaderSource(geometryShaderSource), 
				fragmentShaderSource(fragmentShaderSource) 
			{}

			std::string name;
			ShaderSource vertexShaderSource;
			ShaderSource geometryShaderSource;
			ShaderSource fragmentShaderSource;
		};

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

		/**
		 * \brief Get shader by name
		 * 
		 * \throw std::runtime_error if unable to load
		 * 
		 * \param name Resource name
		 * \return Shader or nullptr if it was not registered
		 */
		std::shared_ptr<Shader> get(const std::string& name);

	private:
		/**
		 * \brief Load shader
		 * 
		 * \throw std::runtime_error if unable to load.
		 * 
		 * \param factoryData Shader factory data
		 * \return Shader
		 */
		std::shared_ptr<Shader> load(const FactoryData& factoryData) const;

		std::shared_ptr<FileManager> m_fileManager;
		std::unordered_map<std::string, FactoryData> m_factoryData;
	};
}