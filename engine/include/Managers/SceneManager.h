#pragma once

#include <memory>
#include <stack>

#include "Core/BaseManager.h"

namespace ej
{
	/**
	 * \brief Scene base class
	 */
	class Scene
	{
	public:
		/**
		 * \brief Default destructor
		 */
		virtual ~Scene() = default;

		/**
		 * \brief Initialization handler
		 * 
		 * This virtual function is called once the scene is added
		 * to the scene manager.
		 */
		virtual void onInit() {}

		/**
		 * \brief Close handler
		 * 
		 * This virtual function is called once the scene is removed
		 * from the scene manager.
		 */
		virtual void onClose() {}

		/**
		 * \brief Handle scene entering
		 * 
		 * This virtual function is called every time this scene becomes
		 * current scene - after initialization or returning from previous 
		 * scene.
		 */
		virtual void onEnter() {}

		/**
		 * \brief Handle scene leaving
		 * 
		 * This virtual function is called every time this scene ceases
		 * to be current.
		 */
		virtual void onLeave() {}

		/**
		 * \brief Handle scene update
		 * 
		 * This function is called every frame while this scene is current.
		 * 
		 * \param dt Time since last frame
		 */
		virtual void onUpdate(const float dt) {}

	protected:
		/**
		 * \brief Get current core object
		 * \return The owner of scene manager, which this scene belongs to
		 */
		const Core& getCore() const {
			return *m_core;
		}

	private:
		friend class SceneManager;

		/**
		 * \brief Current core
		 * 
		 * Assigned during creation by ej::SceneManager
		 */
		const Core* m_core{ nullptr };
	};

	/**
	 * \brief Manage scenes stack
	 */
	class SceneManager final : public BaseManager
	{
	public:
		/**
		 * \brief Constructor
		 * \param core Owner of this manager
		 * \param entryScene First scene, which will be added
		 */
		explicit SceneManager(const Core& core, std::unique_ptr<Scene> entryScene);

		/**
		 * \brief Destructor
		 * 
		 * Removes all scenes using SceneManager::clear()
		 */
		~SceneManager();

		/**
		 * \brief Add new scene on the top of the stack
		 * 
		 * Scene::onInit and Scene::onEnter are called while adding.
		 * 
		 * \param scene New scene
		 */
		void addScene(std::unique_ptr<Scene> scene);

		/**
		 * \brief Remove current scene and add new
		 * 
		 * Warning: after calling this function from current scene
		 * it becomes invalid! You should return right after call.
		 * 
		 * \see SceneManager::removeScene()
		 * \see SceneManager::addScene()
		 * 
		 * \param scene New scene
		 */
		void changeScene(std::unique_ptr<Scene> scene);

		/**
		 * \brief Remove current scene
		 * 
		 * Scene
		 */
		void removeScene();

		/**
		 * \brief Get current scene
		 * 
		 * \throw std::runtime_error if there is no scenes left
		 * 
		 * \return Current scene
		 */
		Scene& peekScene() const;

		/**
		 * \brief Check if there is any scenes left
		 * \return true if there are some scenes
		 */
		bool hasScenes() const;

		/**
		 * \brief Remove all scenes
		 * 
		 * Calls SceneManager::removeScene while there are some scenes
		 */
		void clear();

	private:
		std::stack<std::unique_ptr<Scene>> m_scenes;
	};
}