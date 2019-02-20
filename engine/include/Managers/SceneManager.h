#pragma once

#include <memory>
#include <stack>

#include "Core/BaseManager.h"

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

	protected:
		const Core& getCore() const {
			return *m_core;
		}

	private:
		friend class SceneManager;
		const Core* m_core;
	};

	class SceneManager : public BaseManager, public PointerDefs<SceneManager>
	{
	public:
		explicit SceneManager(const Core& core, std::unique_ptr<Scene> entryScene);
		~SceneManager();

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