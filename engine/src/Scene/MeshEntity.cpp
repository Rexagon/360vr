#include "Scene/MeshEntity.h"

ej::MeshEntity::MeshEntity(Mesh* mesh, Material* material) :
	m_mesh(mesh), m_material(material)
{
}

void ej::MeshEntity::setMesh(Mesh* mesh)
{
	m_mesh = mesh;
}

ej::Mesh* ej::MeshEntity::getMesh() const
{
	return m_mesh;
}

void ej::MeshEntity::setMaterial(Material* material)
{
	m_material = material;
}

ej::Material* ej::MeshEntity::getMaterial() const
{
	return m_material;
}

