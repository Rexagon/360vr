#pragma once

#include <string>
#include <memory>

#include "ManagerLocator.h"

namespace ej
{
	class BaseFileSystem;

	class FileManager : public BaseManager
	{
		class BaseFileSystem
		{
		public:
			virtual ~BaseFileSystem() = default;

			virtual std::string open(const std::string& filename) const = 0;
		};

	public:
		class DiskFileSystem : public BaseFileSystem
		{
		public:
			explicit DiskFileSystem(const std::string& dataFolder = "data/");

			std::string open(const std::string& filename) const override;

		private:
			std::string m_dataFolder;
		};

		explicit FileManager(const ManagerLocator& locator, std::unique_ptr<BaseFileSystem> fileSystem);

		// Clears current filesystem object
		void close();

		// Tries to open specified file
		// Returns vector of bytes if success
		// Throws std::runtime_error if failed
		std::string open(const std::string& filename) const;

	private:
		std::unique_ptr<BaseFileSystem> m_fileSystem;
	};
}