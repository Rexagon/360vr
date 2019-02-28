#pragma once

#include "Resources/Texture.h"
#include "Core/ResourceManager.h"

namespace ej
{
	class FileManager;

	/**
	 * \brief Manage textures
	 */
	class TextureManager final : public ResourceManager<Texture>, public PointerDefs<TextureManager>
	{
	public:
		/**
		 * \param core Owner of this manager
		 */
		explicit TextureManager(const Core& core);

		/**
		 * \brief Register texture loader
		 * \param name Resource name
		 * \param path Texture path
		 * \return this
		 */
		TextureManager* bind(const std::string& name, const std::string& path);

		/**
		 * \brief Get texture by name
		 * 
		 * \throw std::runtime_error if unable to load
		 * 
		 * \param name Resource name
		 * \return Texture or nullptr if it was not registered
		 */
		std::shared_ptr<Texture> get(const std::string& name);

	private:
		/**
		 * \brief Load texture
		 * 
		 * \throw std::runtime_error if unable to load
		 * 
		 * \param path Texture path
		 * \return Texture
		 */
		std::shared_ptr<Texture> load(const std::string& path) const;

		std::shared_ptr<FileManager> m_fileManager;
		std::unordered_map<std::string, std::string> m_factoryData;
	};
}
