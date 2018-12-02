#pragma once

#include "ResourceManager.h"

#include <GL/glew.h>

#include "Texture.h"

namespace ej
{
	class FileManager;

	class TextureManager : public ResourceManager<Texture>
	{
		struct TextureSource
		{
			enum Type { None, File, Raw };

			TextureSource() : type(None) {}

			TextureSource(Type type, const std::string& path) : type(type), fileName(path) {}

			TextureSource(Type type, GLenum handle, GLenum internalFormat, 
				GLenum format, GLenum byteType,  GLenum target) : type(type),
			handle(handle), internalFormat(internalFormat), 
			format(format), byteType(byteType), target(target) {}

			Type type;

			// First type data
			std::string fileName;

			// Second type data
			GLenum handle;
			GLenum internalFormat;
			GLenum format;
			GLenum byteType;
			GLenum target;
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
			FromFile(const std::string& fileName) : 
			TextureSource(File, fileName)
			{}
		};

		struct FromRaw : TextureSource
		{
			FromRaw(GLenum handle,
				GLenum internalFormat = GL_RGB,
				GLenum format = GL_RGB,
				GLenum byteType = GL_UNSIGNED_BYTE,
				GLenum target = GL_TEXTURE_2D) : 
			TextureSource(Raw, handle, internalFormat, format, byteType, target) 
			{}
		};

		explicit TextureManager(const ManagerLocator& locator);

		void bind(const std::string& name, const TextureSource& source);

		std::shared_ptr<Texture> get(const std::string& name);

	private:
		std::shared_ptr<Texture> load(const TextureSource& source) const;

		std::unordered_map<std::string, TextureSource> m_factoryData;
		std::shared_ptr<FileManager> m_fileManager;
	};
}
