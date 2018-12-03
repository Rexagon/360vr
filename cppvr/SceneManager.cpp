#include "SceneManager.h"

using namespace ej;

SceneManager::SceneManager(const Core& core, std::unique_ptr<Scene> entryScene) :
	BaseManager(core)
{
	addScene(std::move(entryScene));
}

SceneManager::~SceneManager()
{
	clear();
}

void SceneManager::addScene(std::unique_ptr<Scene> scene)
{
	if (!m_scenes.empty())
	{
		m_scenes.top()->onLeave();
	}

	scene->m_core = &m_core;

	scene->onInit();
	scene->onEnter();
	m_scenes.push(std::move(scene));
}

void SceneManager::changeScene(std::unique_ptr<Scene> scene)
{
	removeScene();
	addScene(std::move(scene));
}

void SceneManager::removeScene()
{
	if (m_scenes.empty()) return;

	m_scenes.top()->onLeave();
	m_scenes.top()->onClose();

	m_scenes.pop();

	if (m_scenes.empty()) return;

	m_scenes.top()->onEnter();
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

void SceneManager::clear()
{
	while (hasScenes()) {
		removeScene();
		printf("Removed scene");
	}
}
