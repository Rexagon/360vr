#pragma once

#include <memory>
#include <unordered_map>

#include "Core.h"

namespace ej
{
	template<typename T>
	class ResourceManager : public BaseManager
	{
	public:
		explicit ResourceManager(const Core& core) : BaseManager(core) {}
		virtual ~ResourceManager() = default;
		
	protected:
		void insert(const std::string& name, std::shared_ptr<T> resource);
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
