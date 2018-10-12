#pragma once
#include <memory>
#include <stack>

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

class SceneManager
{
public:
	void addScene(std::unique_ptr<Scene> scene);
	void removeScene();
	void changeScene(std::unique_ptr<Scene> scene);

	Scene& peekScene() const;

	bool hasScenes() const;

private:
	std::stack<std::unique_ptr<Scene>> m_scenes;
};