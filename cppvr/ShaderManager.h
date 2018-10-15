#pragma once

#include <unordered_map>
#include <memory>

#include "ResourceManager.h"
#include "Shader.h"

namespace ej
{
	class FileManager;

	class ShaderManager : public ResourceManager<Shader>
	{
		struct ShaderSource
		{
			enum Type { FILE, STRING, NONE };

			ShaderSource() :
				type(Type::NONE)
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
				vertexShaderSource(vertexShaderSource), geometryShaderSource(geometryShaderSource), 
				fragmentShaderSource(fragmentShaderSource) 
			{}

			std::string name;
			ShaderSource vertexShaderSource;
			ShaderSource geometryShaderSource;
			ShaderSource fragmentShaderSource;
		};

	public:
		struct FromFile : ShaderSource
		{
			FromFile(const std::string& source) : ShaderSource(ShaderSource::FILE, source) {}
		};

		struct FromString : ShaderSource
		{
			FromString(const std::string& source) : ShaderSource(ShaderSource::STRING, source) {}
		};

		ShaderManager(const ManagerLocator& locator);

		void bind(const std::string& name, const ShaderSource& vertexShaderSource, 
			const ShaderSource& fragmentShaderSource = ShaderSource(), 
			const ShaderSource& geometryShaderSource = ShaderSource());

		std::shared_ptr<Shader> get(const std::string& name);

	private:
		std::shared_ptr<Shader> load(const FactoryData& factoryData);

		std::unordered_map<std::string, FactoryData> m_factoryData;
		std::shared_ptr<FileManager> m_fileManager;
	};
}