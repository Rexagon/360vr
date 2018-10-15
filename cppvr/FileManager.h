#pragma once

#include <string>
#include <memory>

#include "ManagerLocator.h"

namespace ej
{
	class BaseFileSystem;

	class FileManager : public BaseManager
	{
	public:
		explicit FileManager(const ManagerLocator& locator);

		// Initializes filesystem with specified FileSystem in template argument
		// T - AbstractFileSystem child class type
		// Args - AbstractFileSystem child class constructor arguments
		template<typename T, typename... Args>
		void init(Args&&... args)
		{
			static_assert(std::is_base_of<BaseFileSystem, T>(),
				"Template parameter of FileManager::onInit must be a child of AbstractFileSystem class");

			m_fileSystem = std::make_unique<T>(std::forward<Args>(args)...);
		}

		// Cleares up current filesystem object
		void close();

		// Tries to open specified file
		// Returns vector of bytes if success
		// Throws std::runtime_error if failed
		std::string open(const std::string& filename) const;

	private:
		std::unique_ptr<BaseFileSystem> m_fileSystem;
	};


	// Base class for file sytem
	class BaseFileSystem
	{
	public:
		virtual ~BaseFileSystem() = default;

		virtual std::string open(const std::string& filename) const = 0;
	};

	// Filesystem which uses OS
	class DefaultFileSystem : public BaseFileSystem
	{
	public:
		explicit DefaultFileSystem(const std::string& dataFolder = "data/");

		std::string open(const std::string& filename) const override;

	private:
		std::string m_dataFolder;
	};
}