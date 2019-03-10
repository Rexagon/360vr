#pragma once

#include "Entity.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"

namespace ej
{
	class MeshEntity final : public Entity
	{
	public:
		explicit MeshEntity(Mesh* mesh = nullptr, Material* material = nullptr);

		void setMesh(Mesh* mesh);
		Mesh* getMesh() const;

		void setMaterial(Material* material);
		Material* getMaterial() const;

	private:
		Mesh* m_mesh = nullptr;
		Material* m_material = nullptr;
	};
}