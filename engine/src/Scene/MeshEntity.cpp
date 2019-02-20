#include "Scene/MeshEntity.h"

ej::MeshEntity::MeshEntity(Mesh::ptr mesh, std::shared_ptr<Material> material) :
	m_mesh(mesh), m_material(material)
{
}

void ej::MeshEntity::setMesh(Mesh::ptr mesh)
{
	m_mesh = mesh;
}

ej::Mesh* ej::MeshEntity::getMesh() const
{
	return m_mesh.get();
}

void ej::MeshEntity::setMaterial(std::shared_ptr<Material> material)
{
	m_material = material;
}

ej::Material* ej::MeshEntity::getMaterial() const
{
	return m_material.get();
}

