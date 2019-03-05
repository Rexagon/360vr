#include "Managers/SceneManager.h"

#include <stdexcept>

ej::SceneManager::SceneManager(const Core& core, std::unique_ptr<Scene> entryScene) :
	BaseManager(core)
{
	addScene(std::move(entryScene));
}

ej::SceneManager::~SceneManager()
{
	clear();
}

void ej::SceneManager::addScene(std::unique_ptr<Scene> scene)
{
	if (!m_scenes.empty()) {
		m_scenes.top()->onLeave();
	}

	scene->m_core = &m_core;

	scene->onInit();
	scene->onEnter();
	m_scenes.push(std::move(scene));
}

void ej::SceneManager::changeScene(std::unique_ptr<Scene> scene)
{
	removeScene();
	addScene(std::move(scene));
}

void ej::SceneManager::removeScene()
{
	if (m_scenes.empty()) {
		return;
	}

	m_scenes.top()->onLeave();
	m_scenes.top()->onClose();

	m_scenes.pop();

	if (m_scenes.empty()) {
		return;
	}

	m_scenes.top()->onEnter();
}

ej::Scene & ej::SceneManager::peekScene() const
{
	if (m_scenes.empty()) {
		throw std::runtime_error("Peeking on empty SceneManager");
	}

	return *m_scenes.top();
}

bool ej::SceneManager::hasScenes() const
{
	return !m_scenes.empty();
}

void ej::SceneManager::clear()
{
	while (hasScenes()) {
		removeScene();
		printf("Removed scene");
	}
}
