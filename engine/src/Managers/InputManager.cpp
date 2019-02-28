#include "Managers/InputManager.h"

#include "Core/Core.h"

bool ej::details::isKeyValid(const Key key)
{
	return key > Key::Unknown && key < Key::KeyCount;
}

bool ej::details::isMouseButtonValid(const MouseButton button)
{
	return button < MouseButton::ButtonCount;
}


ej::InputManager::InputManager(const Core& core) :
	BaseManager(core)
{
	m_windowManager = core.get<WindowManager>();
}

void ej::InputManager::updateState()
{
	m_anyKeyUp = false;
	m_anyKeyDown = false;
	m_lastKeysState = m_currentKeysState;

	m_anyMouseButtonUp = false;
	m_anyMouseButtonDown = false;
	m_lastMouseButtonsState = m_currentMouseButtonsState;

	m_lastMousePosition = m_currentMousePosition;

	m_mouseWheelDelta = 0.0f;
}

void ej::InputManager::handleEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		if (details::isKeyValid(event.key.code)) {
			m_anyKeyDown = true;
			m_currentKeysState.set(event.key.code);
		}
		break;

	case sf::Event::KeyReleased:
		if (details::isKeyValid(event.key.code)) {
			m_anyKeyUp = true;
			m_currentKeysState.reset(event.key.code);
		}
		break;

	case sf::Event::MouseButtonPressed:
		if (details::isMouseButtonValid(event.mouseButton.button)) {
			m_anyMouseButtonDown = true;
			m_currentMouseButtonsState.set(event.mouseButton.button);
		}
		break;

	case sf::Event::MouseButtonReleased:
		if (details::isMouseButtonValid(event.mouseButton.button)) {
			m_anyMouseButtonUp = true;
			m_currentMouseButtonsState.reset(event.mouseButton.button);
		}
		break;

	case sf::Event::MouseMoved:
		m_currentMousePosition = glm::ivec2(event.mouseMove.x, event.mouseMove.y);
		break;

	case sf::Event::MouseWheelScrolled:
		m_mouseWheelDelta = event.mouseWheelScroll.delta;
		break;

	default: 
		break;
	}
}

bool ej::InputManager::getKey(const Key key) const
{
	return details::isKeyValid(key) &&
		m_currentKeysState.test(static_cast<size_t>(key));
}

bool ej::InputManager::getKeyDown(const Key key) const
{
	return details::isKeyValid(key) &&
		!m_lastKeysState.test(key) &&
		m_currentKeysState.test(key);
}

bool ej::InputManager::getKeyUp(const Key key) const
{
	return details::isKeyValid(key) &&
		m_lastKeysState.test(key) &&
		!m_currentKeysState.test(key);
}

bool ej::InputManager::getAnyKey() const
{
	return m_currentKeysState.any();
}

bool ej::InputManager::getAnyKeyDown() const
{
	return m_anyKeyDown;
}

bool ej::InputManager::getAnyKeyUp() const
{
	return m_anyKeyUp;
}

bool ej::InputManager::getMouseButton(const MouseButton button) const
{
	return details::isMouseButtonValid(button) &&
		m_currentMouseButtonsState.test(button);
}

bool ej::InputManager::getMouseButtonDown(const MouseButton button) const
{
	return details::isMouseButtonValid(button) &&
		!m_lastMouseButtonsState.test(button) &&
		m_currentMouseButtonsState.test(button);
}

bool ej::InputManager::getMouseButtonUp(const MouseButton button) const
{
	return details::isMouseButtonValid(button) &&
		m_lastMouseButtonsState.test(button) &&
		!m_currentMouseButtonsState.test(button);
}

float ej::InputManager::getAxis(const std::string& name)
{
	const auto it = m_axes.find(name);
	if (it == m_axes.end() || it->second == nullptr) {
		return 0.0f;
	}

	return it->second->getValue();
}

void ej::InputManager::setMouseCursorVisible(const bool visible)
{
	m_windowManager->getWindow().setMouseCursorVisible(visible);
}

void ej::InputManager::setMousePosition(const glm::ivec2 & position)
{
	m_currentMousePosition = position;
	sf::Mouse::setPosition(sf::Vector2i(position.x, position.y),
		m_windowManager->getWindow());
}

glm::ivec2 ej::InputManager::getMousePositionDelta() const
{
	return m_currentMousePosition - m_lastMousePosition;
}

glm::ivec2 ej::InputManager::getMousePosition() const
{
	return m_currentMousePosition;
}
