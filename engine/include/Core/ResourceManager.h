#pragma once

#include <memory>
#include <unordered_map>

#include "Core.h"

namespace ej
{
	/**
	 * \brief Base class for lazy loading resources
	 * \tparam T Type of resource
	 */
	template<typename T>
	class ResourceManager : public BaseManager
	{
	public:
		/**
		 * \brief Default constructor
		 * \param core Owner of this manager
		 */
		explicit ResourceManager(const Core& core) : BaseManager(core) {}

		/**
		 * \brief Default destructor
		 */
		virtual ~ResourceManager() = default;
		
	protected:
		/**
		 * \brief Save \a resource, labeled with \a name
		 * 
		 * If resource with same \a name was provided, new \a resource
		 * will be assigned to old.
		 * 
		 * \param name Resource name
		 * \param resource Resource data
		 */
		void insert(const std::string& name, std::shared_ptr<T> resource);

		/**
		 * \brief Find resource, labeled with \a name
		 * \param name Resource name
		 * \return Resource. Can be nullptr if not found
		 */
		std::shared_ptr<T> find(const std::string& name);

	private:
		std::unordered_map<std::string, std::shared_ptr<T>> m_resources;
	};

	template<typename T>
	void ResourceManager<T>::insert(const std::string& name, std::shared_ptr<T> resource)
	{
		m_resources.insert_or_assign(name, resource);
	}

	template<typename T>
	std::shared_ptr<T> ResourceManager<T>::find(const std::string& name)
	{
		auto it = m_resources.find(name);

		if (it == m_resources.end()) {
			return nullptr;
		}

		return it->second;
	}
}

/**
 * \class ej::ResourceManager
 * \ingroup Core
 */