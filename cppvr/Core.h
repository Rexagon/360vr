#pragma once

#include <unordered_map>
#include <typeindex>
#include <cassert>
#include <memory>

#include "BaseManager.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "SceneManager.h"

namespace core
{

class Core
{
public:
	// Starts main game loop
	// Before calling this, you must provide at least this managers:
	//   - WindowManager
	//   - InputManager
	//   - InputManager
	void run() const;

	template<typename T, typename... Args>
	void provideManager(Args&&... args);

	template<typename T>
	std::shared_ptr<T> getManager() const;

	template<typename T>
	bool hasManager() const;

private:
	std::unordered_map<std::type_index, std::shared_ptr<BaseManager>> m_managers;
};

template<typename T, typename ...Args>
inline void Core::provideManager(Args && ...args)
{
	static_assert(std::is_base_of<BaseManager, T>(), "Provided manager must be derived from BaseManager");

	auto manager = std::static_pointer_cast<BaseManager>(std::make_shared<T>(std::forward<Args>(args)...));
	manager->m_core = this;
	m_managers.insert_or_assign(std::type_index(typeid(T)), std::move(manager));
}

template<typename T>
inline std::shared_ptr<T> Core::getManager() const
{
	static_assert(std::is_base_of<BaseManager, T>(), "Provided manager must be derived from BaseManager");

	auto it = m_managers.find(std::type_index(typeid(T)));
	assert(it != m_managers.end());
	return std::dynamic_pointer_cast<T>(it->second);
}

template<typename T>
inline bool Core::hasManager() const
{
	static_assert(std::is_base_of<BaseManager, T>(), "Provided manager must be derived from BaseManager");

	return m_managers.find(std::type_index(typeid(T))) != m_managers.end();
}

template<typename T>
inline std::shared_ptr<T> RequireManager::get() const
{
	assert(m_core != nullptr);
	return m_core->getManager<T>();
}

}
