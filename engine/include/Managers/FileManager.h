#pragma once

#include <string>
#include <memory>

#include "Core/BaseManager.h"

namespace ej
{
	class BaseFileSystem;

	/**
	 * \brief Manage access to filesystem
	 */
	class FileManager final : public BaseManager, public PointerDefs<FileManager>
	{
		/**
		 * \brief Base file provider
		 */
		class BaseFileSystem
		{
		public:
			/**
			 * \brief Default destructor
			 */
			virtual ~BaseFileSystem() = default;

			/**
			 * \brief Read file content
			 * 
			 * This is a pure virtual function that has to be implemented
			 * by the derived class to define how to read file content.
			 * 
			 * \param filename File name in current system
			 * \return File content
			 */
			virtual std::string open(const std::string& filename) const = 0;
		};

	public:
		/**
		 * \brief Real file system
		 */
		class DiskFileSystem : public BaseFileSystem
		{
		public:
			/**
			 * \brief Create disk filesystem with specified root folder
			 * \param dataFolder Root folder
			 */
			explicit DiskFileSystem(const std::string& dataFolder = "data/");

			/**
			 * \brief Read file content from disk
			 * 
			 * \throw std::runtime_error if file was not found.
			 * 
			 * \param filename File name in local filesystem
			 * \return File content
			 */
			std::string open(const std::string& filename) const override;

		private:
			std::string m_dataFolder;
		};

		/**
		 * \brief Create file manager, which use specified file system
		 * \param core Owner of this manager
		 * \param fileSystem Filesystem, used to read file content
		 */
		explicit FileManager(const Core& core, std::unique_ptr<BaseFileSystem> fileSystem);

		/**
		 * \brief Read file content
		 * 
		 * \throw std::runtime_error if file was not found.
		 * 
		 * \param filename File name
		 * \return File content
		 */
		std::string open(const std::string& filename) const;

	private:
		std::unique_ptr<BaseFileSystem> m_fileSystem;
	};
}
