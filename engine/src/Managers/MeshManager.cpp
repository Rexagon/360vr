#include "Managers/MeshManager.h"

ej::MeshManager::MeshManager(const Core& core) :
	ResourceManager(core)
{
}

ej::MeshManager* ej::MeshManager::bind(const std::string& name, const Loader& loader)
{
	m_factoryData.emplace(name, loader);

	return this;
}

ej::PointerDefs<ej::Mesh>::ptr ej::MeshManager::get(const std::string& name)
{
	Mesh::ptr result = find(name);
	if (result == nullptr) {
		const auto it = m_factoryData.find(name);
		if (it != m_factoryData.end()) {
			result = load(it->second);
			insert(name, result);
		}
	}
	return result;
}

ej::PointerDefs<ej::Mesh>::ptr ej::MeshManager::load(const Loader& loader) const
{
	auto mesh = std::make_shared<Mesh>();
	mesh->init(loader());

	return mesh;
}
