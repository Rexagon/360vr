#pragma once

#include "Resources/Texture.h"
#include "Core/ResourceManager.h"

namespace ej
{
	class FileManager;

	/**
	 * \brief Manage textures
	 */
	class TextureManager final : public ResourceManager<TextureManager, Texture>
	{
	public:
		/**
		 * \param core Owner of this manager
		 */
		explicit TextureManager(const Core& core);

	private:
		/**
		 * \brief Load texture
		 * 
		 * \throw std::runtime_error if unable to load
		 * 
		 * \param path Texture path
		 * \return Texture
		 */
		std::unique_ptr<Texture> load(const std::string& path) override;

		FileManager* m_fileManager = nullptr;
	};
}
