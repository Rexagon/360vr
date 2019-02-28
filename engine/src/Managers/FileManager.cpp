#include "Managers/FileManager.h"

#include <fstream>

ej::FileManager::FileManager(const Core & core, std::unique_ptr<BaseFileSystem> fileSystem) :
	BaseManager(core), m_fileSystem(std::move(fileSystem))
{
}

std::string ej::FileManager::open(const std::string & filename) const
{
	if (m_fileSystem == nullptr) {
		throw std::runtime_error("Unable to open file: \"" + filename + "\". Filesystem wasn't initialized");
	}

	return m_fileSystem->open(filename);
}


ej::FileManager::DiskFileSystem::DiskFileSystem(const std::string & dataFolder) :
	m_dataFolder(dataFolder)
{
}

std::string ej::FileManager::DiskFileSystem::open(const std::string & filename) const
{
	std::ifstream file(m_dataFolder + filename, std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("Unable to open file: \"" + m_dataFolder + filename + "\". File is missing");
	}

	std::string result;

	file.seekg(0, std::ios::end);
	result.resize(file.tellg());

	file.seekg(0, std::ios::beg);
	file.read(&result[0], result.size());

	file.close();

	return result;
}
