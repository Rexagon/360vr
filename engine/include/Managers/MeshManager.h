#pragma once

#include <functional>

#include "Resources/Mesh.h"
#include "Core/ResourceManager.h"

namespace ej
{
	/**
	 * \brief Manage meshes
	 */
	class MeshManager final : public ResourceManager<MeshManager, Mesh, std::function<MeshGeometry()>>
	{
	public:

		/**
		 * \param core Owner of this manager
		 */
		explicit MeshManager(const Core& core);

	private:
		/**
		 * \brief Load mesh
		 * \param loader Loader function
		 * \return Mesh
		 */
		std::unique_ptr<Mesh> load(const std::function<MeshGeometry()>& loader) override;
	};
}
