#pragma once

#include <unordered_map>
#include <typeindex>
#include <cassert>
#include <memory>

#include "BaseManager.h"

namespace ej
{
	class ManagerLocator final
	{
	public:
		template<typename T, typename... Args>
		void provide(Args&&... args);

		template<typename T>
		std::shared_ptr<T> get() const;

		template<typename T>
		bool has() const;

	private:
		std::unordered_map<std::type_index, std::shared_ptr<BaseManager>> m_managers;
	};

	template<typename T, typename ...Args>
	inline void ManagerLocator::provide(Args && ...args)
	{
		static_assert(std::is_base_of<BaseManager, T>(), "Provided manager must be derived from BaseManager");

		auto manager = std::static_pointer_cast<BaseManager>(std::make_shared<T>(*this, std::forward<Args>(args)...));
		m_managers.insert_or_assign(std::type_index(typeid(T)), std::move(manager));
	}

	template<typename T>
	inline std::shared_ptr<T> ManagerLocator::get() const
	{
		static_assert(std::is_base_of<BaseManager, T>(), "Provided manager must be derived from BaseManager");

		auto it = m_managers.find(std::type_index(typeid(T)));
		assert(it != m_managers.end());
		return std::dynamic_pointer_cast<T>(it->second);
	}

	template<typename T>
	inline bool ManagerLocator::has() const
	{
		static_assert(std::is_base_of<BaseManager, T>(), "Provided manager must be derived from BaseManager");

		return m_managers.find(std::type_index(typeid(T))) != m_managers.end();
	}
}
