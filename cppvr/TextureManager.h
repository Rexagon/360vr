#pragma once

#include "ResourceManager.h"

#include <SFML/Graphics/Texture.hpp>
#include <GL/glew.h>

namespace ej
{
	class FileManager;

	class TextureManager : public ResourceManager<sf::Texture>
	{
		struct TextureSource
		{
			enum Type { None, File, Raw };

			TextureSource() : type(None) {}

			TextureSource(Type type, const std::string& path) : type(type) {}
			TextureSource(Type type, GLenum handle) : handle(handle) {}

			Type type;
			std::string fileName;
			GLenum handle;
		};

		struct FactoryData
		{
			FactoryData(const std::string& name, const TextureSource& source) :
				name(name), source(source) {}

			std::string name;
			TextureSource source;
		};

	public:
		struct FromFile : TextureSource
		{
			FromFile(const std::string& fileName): TextureSource(File, fileName) {}
		};

		struct FromRaw : TextureSource
		{
			FromRaw(GLenum handle): TextureSource(Raw, handle) {}
		};

		explicit TextureManager(const ManagerLocator& locator);

		void bind(const std::string& name, const TextureSource& source);

		std::shared_ptr<sf::Texture> get(const std::string& name);

	private:
		std::shared_ptr<sf::Texture> load(const TextureSource& source) const;

		std::unordered_map<std::string, TextureSource> m_factoryData;
		std::shared_ptr<FileManager> m_fileManager;
	};
}
