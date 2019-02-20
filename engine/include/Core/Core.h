#pragma once

#include <cassert>
#include <typeindex>
#include <unordered_map>

#include <SFML/Window/Event.hpp>

#include "BaseManager.h"

namespace ej 
{
	class Core
	{
	public:
		Core();
		virtual ~Core() = default;

		void run();
		void stop();

		template<typename T, typename... Args>
		std::shared_ptr<T> provide(Args&&... args);

		template<typename T>
		std::shared_ptr<T> get() const;

		template<typename T>
		bool has() const;

	protected:
		virtual void onHandleEvent(const sf::Event& event) = 0;

		virtual void onBeforeUpdate(float dt) {}
		virtual void onUpdate(float dt) = 0;

	private:
		bool m_isRunning;
		std::unordered_map<std::type_index, std::shared_ptr<BaseManager>> m_managers;
	};

	template<typename T, typename ...Args>
	std::shared_ptr<T> Core::provide(Args && ...args)
	{
		static_assert(std::is_base_of<BaseManager, T>(), "Provided manager must be derived from BaseManager");

		auto manager = std::make_shared<T>(*this, std::forward<Args>(args)...);
		m_managers.insert_or_assign(std::type_index(typeid(T)), std::static_pointer_cast<BaseManager>(manager));

		return manager;
	}

	template<typename T>
	std::shared_ptr<T> Core::get() const
	{
		static_assert(std::is_base_of<BaseManager, T>(), "Provided manager must be derived from BaseManager");

		const auto it = m_managers.find(std::type_index(typeid(T)));
		assert(it != m_managers.end());
		return std::dynamic_pointer_cast<T>(it->second);
	}

	template<typename T>
	bool Core::has() const
	{
		static_assert(std::is_base_of<BaseManager, T>(), "Provided manager must be derived from BaseManager");

		return m_managers.find(std::type_index(typeid(T))) != m_managers.end();
	}
}

#define EJ_MAIN(Core) int main() { \
	try { \
		Core game; \
		game.run(); \
	} catch (const std::exception& e) { \
		std::cout << e.what() << std::endl; \
		std::cin.get(); \
	} \
	return 0; \
}