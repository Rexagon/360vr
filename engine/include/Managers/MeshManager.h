#pragma once

#include <functional>

#include "Resources/Mesh.h"
#include "Core/ResourceManager.h"

namespace ej
{
	class FileManager;

	/**
	 * \brief Manage meshes
	 */
	class MeshManager final : public ResourceManager<MeshManager, Mesh>
	{
	public:

		/**
		 * \param core Owner of this manager
		 */
		explicit MeshManager(const Core& core);

	private:
		/**
		 * \brief Load mesh
		 * \param filename OBJ file name
		 * \return Mesh
		 */
		std::unique_ptr<Mesh> load(const std::string& filename) override;

		FileManager* m_fileManager = nullptr;
	};
}
