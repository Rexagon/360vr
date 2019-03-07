#pragma once

#include <cassert>
#include <typeindex>
#include <unordered_map>

#include <SFML/Window/Event.hpp>

#include "BaseManager.h"

namespace ej 
{
	/**
	 * \brief Base application class
	 * 
	 * Give access to all provided managers.
	 * Some handlers may be overridden.
	 */
	class Core
	{
	public:
		/**
		 * \brief Default destructor
		 * 
		 * All provided managers will be destroyed.
		 */
		virtual ~Core() = default;

		/**
		 * \brief Start main application loop
		 * 
		 * Function blocks current thread until application is
		 * closed using Core::stop().
		 * 
		 * Warning: ej::WindowManager must be provided before
		 * calling this function.
		 * 
		 * If application is already running, this function does
		 * nothing.
		 */
		void run();

		/**
		 * \brief Stop application loop
		 * 
		 * Application will stop after current loop iteration.
		 * 
		 * If application is already stopped, this function does
		 * nothing.
		 */
		void stop();

		/**
		 * \brief Provide manager
		 * 
		 * Register and construct provided manager with specified
		 * arguments.
		 * 
		 * If manager of same type was already provided, new manager
		 * will be assigned to old.
		 * 
		 * \tparam T Manager type. Must be child of ej::BaseManager
		 * \tparam Args Manager constructor arguments types
		 * \param args Manager constructor arguments
		 * \return Created manager
		 */
		template<typename T, typename... Args>
		T* provide(Args&&... args);

		/**
		 * \brief Find manager
		 * 
		 * If no manager were found, application will stop.
		 * Manager of type \a T must be provided before calling this
		 * function.
		 * 
		 * \tparam T Manager type. Must be child of ej::BaseManager
		 * \return Manager object
		 */
		template<typename T>
		T* get() const;

		/**
		 * \brief Check if manager of type \a T has been provided
		 * \tparam T Manager type. Must be child of ej::BaseManager
		 * \return true if found
		 */
		template<typename T>
		bool has() const;

	protected:
		/**
		 * \brief Handle window event
		 * 
		 * This is a pure virtual function that has to be implemented
		 * by the derived class to define how window events should be 
		 * handled.
		 * 
		 * It is called every frame while application is running for 
		 * each event polled from window since last frame.
		 * 
		 * \param event Window event
		 */
		virtual void onHandleEvent(const sf::Event& event) = 0;

		/**
		 * \brief Do some work at the very beginning of frame
		 * 
		 * This is a virtual function that can be implemented by the 
		 * derived class to define work at the very beginning of frame.
		 * 
		 * It is called at the beginning of every frame while 
		 * application is running, before any event was handled.
		 * 
		 * \param dt Time since last frame
		 * 
		 * \see onHandleEvent, onUpdate
		 */
		virtual void onBeforeUpdate(float dt) {}
		
		/**
		 * \brief Do some work at the end of frame
		 * 
		 * This is a pure virtual function that has to be implemented
		 * by the derived class to define work at the end of frame.
		 * 
		 * It is called at the end of every frame after all events
		 * were handled.
		 * 
		 * \param dt Time since last frame
		 */
		virtual void onUpdate(float dt) = 0;

	private:
		bool m_isRunning = false;
		std::unordered_map<std::type_index, std::unique_ptr<BaseManager>> m_managers;
	};

	template<typename T, typename ...Args>
	T* Core::provide(Args && ...args)
	{
		static_assert(std::is_base_of<BaseManager, T>(), "Provided manager must be derived from BaseManager");

		auto manager = new T(*this, std::forward<Args>(args)...);
		m_managers.insert_or_assign(std::type_index(typeid(T)), std::unique_ptr<BaseManager>(manager));

		return manager;
	}

	template<typename T>
	T* Core::get() const
	{
		static_assert(std::is_base_of<BaseManager, T>(), "Provided manager must be derived from BaseManager");

		const auto it = m_managers.find(std::type_index(typeid(T)));
		assert(it != m_managers.end());
		return dynamic_cast<T*>(it->second.get());
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