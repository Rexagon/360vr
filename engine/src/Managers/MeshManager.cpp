#include "Managers/MeshManager.h"

ej::MeshManager::MeshManager(const Core& core) :
	ResourceManager(core)
{
}

std::unique_ptr<ej::Mesh> ej::MeshManager::load(const std::function<MeshGeometry()>& loader)
{
	auto mesh = std::make_unique<Mesh>(m_core);
	mesh->init(loader());

	return mesh;
}