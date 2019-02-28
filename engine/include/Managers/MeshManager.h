#pragma once

#include <functional>
#include <unordered_map>

#include "Resources/Mesh.h"
#include "Core/ResourceManager.h"

namespace ej
{
	/**
	 * \brief Manage meshes
	 */
	class MeshManager final : public ResourceManager<Mesh>, public PointerDefs<MeshManager>
	{
	public:
		/**
		 * \brief Provider of mesh geometry
		 * 
		 * \see ej::MeshGeometry
		 */
		using Loader = std::function<MeshGeometry()>;

		/**
		 * \param core Owner of this manager
		 */
		explicit MeshManager(const Core& core);

		/**
		 * \brief Register mesh loader
		 * \param name Resource name
		 * \param loader Loader function
		 * \return this
		 */
		MeshManager* bind(const std::string& name, const Loader& loader);

		/**
		 * \brief Get mesh by name
		 * \param name Resource name
		 * \return Mesh or nullptr it was not registered
		 */
		Mesh::ptr get(const std::string& name);

	private:
		/**
		 * \brief Load mesh
		 * \param loader Loader function
		 * \return Mesh
		 */
		Mesh::ptr load(const Loader& loader) const;

		std::unordered_map<std::string, Loader> m_factoryData;
	};
}
