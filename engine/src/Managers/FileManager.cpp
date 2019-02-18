#include "Managers/FileManager.h"

#include <fstream>

using namespace ej;

FileManager::FileManager(const Core & core, std::unique_ptr<BaseFileSystem> fileSystem) :
	BaseManager(core), m_fileSystem(std::move(fileSystem))
{
}

void FileManager::close()
{
	m_fileSystem.reset(nullptr);
}

std::string FileManager::open(const std::string & filename) const
{
	if (m_fileSystem == nullptr) {
		throw std::runtime_error("Unable to open file: \"" + filename + "\". Filesystem wasn't initialized");
	}
	else {
		return m_fileSystem->open(filename);
	}
}


FileManager::DiskFileSystem::DiskFileSystem(const std::string & dataFolder) :
	m_dataFolder(dataFolder)
{
}

std::string FileManager::DiskFileSystem::open(const std::string & filename) const
{
	std::ifstream file(m_dataFolder + filename, std::ios::binary);

	if (file.is_open()) {
		std::string result;

		file.seekg(0, std::ios::end);
		result.resize(file.tellg());

		file.seekg(0, std::ios::beg);
		file.read(&result[0], result.size());

		file.close();

		return result;
	}
	else {
		throw std::runtime_error("Unable to open file: \"" + m_dataFolder + filename + "\". File is missing");
	}
}