#include "SceneManager.h"

void SceneManager::addScene(std::unique_ptr<Scene> scene)
{
	if (!m_scenes.empty())
	{
		m_scenes.top()->onLeave();
	}

	scene->onInit();
	scene->onEnter();
	m_scenes.push(std::move(scene));
}

void SceneManager::removeScene()
{
	if (m_scenes.empty()) return;

	m_scenes.top()->onLeave();
	m_scenes.top()->onClose();

	if (m_scenes.empty()) return;

	m_scenes.top()->onEnter();
}

void SceneManager::changeScene(std::unique_ptr<Scene> scene)
{
	removeScene();
	addScene(std::move(scene));
}

Scene & SceneManager::peekScene() const
{
	if (m_scenes.empty())
	{
		throw std::runtime_error("Peeking on empty SceneManager");
	}

	return *m_scenes.top();
}

bool SceneManager::hasScenes() const
{
	return !m_scenes.empty();
}
