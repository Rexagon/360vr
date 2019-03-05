#pragma once

#include <memory>
#include <unordered_map>

#include "Core.h"

namespace ej
{
	/**
	 * \brief Base class for lazy loading resources
	 * \tparam M Manager, which derives from this class (CRTP)
	 * \tparam T Type of resource
	 * \tparam F Factory data type. String by default
	 */
	template<typename M, typename T, typename F = std::string>
	class ResourceManager : public BaseManager
	{
	public:
		using ResourceType = T;
		using FactoryDataType = F;

		/**
		 * \param core Owner of this manager
		 */
		explicit ResourceManager(const Core& core) : BaseManager(core) {}

		/**
		 * \brief Default destructor
		 */
		virtual ~ResourceManager() = default;

		M& bind(const std::string& name, const FactoryDataType& factoryData);

		/**
		 * \brief Get resource by name
		 *
		 * \throw std::runtime_error if unable to load
		 *
		 * \param name Resource name
		 * \return Font or nullptr if it was not registered
		 */
		T* get(const std::string& name);
		
	protected:
		/**
		 * \brief Save \a resource, labeled with \a name
		 * 
		 * If resource with same \a name was provided, new \a resource
		 * will be assigned to old.
		 * 
		 * \param name Resource name
		 * \param resource Resource data. Will be moved
		 */
		void insert(const std::string& name, std::unique_ptr<T>& resource);

		/**
		 * \brief Find resource, labeled with \a name
		 * \param name Resource name
		 * \return Resource. Can be nullptr if not found
		 */
		T* find(const std::string& name);

		/**
		 * \brief Load resource
		 *
		 * \throw std::runtime_error if unable to load.
		 *
		 * \param factoryData Data, needed to create resource
		 * \return Resource
		 */
		virtual std::unique_ptr<T> load(const F& factoryData) = 0;

		std::unordered_map<std::string, std::unique_ptr<T>> m_resources;
		std::unordered_map<std::string, F> m_factoryData;
	};
}


template <typename M, typename T, typename F>
M& ej::ResourceManager<M, T, F>::bind(const std::string& name, const FactoryDataType& factoryData)
{
	m_factoryData.emplace(name, factoryData);
	return *dynamic_cast<M*>(this);
}

template <typename M, typename T, typename F>
T* ej::ResourceManager<M, T, F>::get(const std::string& name)
{
	auto result = find(name);
	if (result == nullptr) {
		const auto it = m_factoryData.find(name);
		if (it != m_factoryData.end()) {
			auto data = load(it->second);
			result = data.get();
			insert(name, data);
		}
	}

	return result;
}

template<typename M, typename T, typename F>
void ej::ResourceManager<M, T, F>::insert(const std::string& name, std::unique_ptr<T>& resource)
{
	m_resources.insert_or_assign(name, std::move(resource));
}

template<typename M, typename T, typename F>
T* ej::ResourceManager<M, T, F>::find(const std::string& name)
{
	auto it = m_resources.find(name);
	if (it == m_resources.end()) {
		return nullptr;
	}

	return it->second.get();
}
