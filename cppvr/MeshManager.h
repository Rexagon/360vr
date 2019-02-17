#pragma once

#include <functional>
#include <unordered_map>

#include "Mesh.h"
#include "ResourceManager.h"

namespace ej
{
	class MeshManager : public ResourceManager<Mesh>, public PointerDefs<MeshManager>
	{
	public:
		using Loader = std::function<MeshGeometry()>;

		explicit MeshManager(const Core& core);

		MeshManager* bind(const std::string& name, const Loader& loader);

		Mesh::ptr get(const std::string& name);

	private:
		Mesh::ptr load(const Loader& loader) const;

		std::unordered_map<std::string, Loader> m_factoryData;
	};
}
