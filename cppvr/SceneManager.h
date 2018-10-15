#pragma once

#include <memory>
#include <stack>

#include "BaseManager.h"

namespace ej
{
	class Scene
	{
	public:
		virtual ~Scene() = default;

		virtual void onInit() {}
		virtual void onClose() {}

		virtual void onEnter() {}
		virtual void onLeave() {}

		virtual void onUpdate(const float dt) {}
	};

	class SceneManager : public BaseManager
	{
	public:
		SceneManager(const ManagerLocator& locator, std::unique_ptr<Scene> entryScene);

		void addScene(std::unique_ptr<Scene> scene);
		void changeScene(std::unique_ptr<Scene> scene);
		void removeScene();

		Scene& peekScene() const;

		bool hasScenes() const;

		void clear();

	private:
		std::stack<std::unique_ptr<Scene>> m_scenes;
	};
}