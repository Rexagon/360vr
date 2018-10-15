#pragma once

#include <unordered_map>
#include <memory>

#include "ManagerLocator.h"

namespace ej
{
	template<typename T>
	class ResourceManager : public BaseManager
	{
	public:
		ResourceManager(const ManagerLocator& locator) : BaseManager(locator) {}
		virtual ~ResourceManager() {}
		
	protected:
		void insert(const std::string& name, std::shared_ptr<T> resource);
		std::shared_ptr<T> find(const std::string& name);

	private:
		std::unordered_map<std::string, std::shared_ptr<T>> m_resources;
	};

	template<typename T>
	inline void ResourceManager<T>::insert(const std::string & name, std::shared_ptr<T> resource)
	{
		m_resources.insert_or_assign(name, resource);
	}

	template<typename T>
	inline std::shared_ptr<T> ResourceManager<T>::find(const std::string & name)
	{
		auto it = m_resources.find(name);

		if (it == m_resources.end()) {
			return nullptr;
		}

		return it->second;
	}
}