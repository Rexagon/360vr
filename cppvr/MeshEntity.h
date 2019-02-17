#pragma once

#include "Mesh.h"
#include "Entity.h"
#include "Material.h"

namespace ej
{
	class MeshEntity : public Entity, public PointerDefs<MeshEntity>
	{
	public:
		MeshEntity(Mesh::ptr mesh, std::shared_ptr<Material> material);

		void setMesh(Mesh::ptr mesh);
		Mesh* getMesh() const;

		void setMaterial(std::shared_ptr<Material> material);
		Material* getMaterial() const;

	private:
		Mesh::ptr m_mesh;
		std::shared_ptr<Material> m_material;
	};
}