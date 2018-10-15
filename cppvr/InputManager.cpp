#include "InputManager.h"

using namespace ej;

bool details::isKeyValid(const Key key)
{
	return key > Key::Unknown && key < Key::KeyCount;
}

bool details::isMouseButtonValid(const MouseButton button)
{
	return button > -1 && button < MouseButton::ButtonCount;
}


InputManager::InputManager(const ManagerLocator& locator) :
	BaseManager(locator)
{
}

void InputManager::updateState()
{
	m_lastKeysState = m_currentKeysState;
	m_lastMouseButtonsState = m_currentMouseButtonsState;
}

void InputManager::handleEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		if (details::isKeyValid(event.key.code))
		{
			m_currentKeysState.set(event.key.code);
		}
		break;
	case sf::Event::KeyReleased:
		if (details::isKeyValid(event.key.code))
		{
			m_currentKeysState.reset(event.key.code);
		}
		break;
	default: 
		break;
	}
}

bool InputManager::getKey(const Key key) const
{
	return details::isKeyValid(key) &&
		m_currentKeysState.test(static_cast<size_t>(key));
}

bool InputManager::getKeyDown(const Key key) const
{
	return details::isKeyValid(key) &&
		!m_lastKeysState.test(key) &&
		m_currentKeysState.test(key);
}

bool InputManager::getKeyUp(const Key key) const
{
	return details::isKeyValid(key) &&
		m_lastKeysState.test(key) &&
		!m_currentKeysState.test(key);
}

bool InputManager::getMouseButton(const MouseButton button) const
{
	return details::isMouseButtonValid(button) &&
		m_currentMouseButtonsState.test(button);
}

bool InputManager::getMouseButtonDown(const MouseButton button) const
{
	return details::isMouseButtonValid(button) &&
		!m_lastMouseButtonsState.test(button) &&
		m_currentMouseButtonsState.test(button);
}

bool InputManager::getMouseButtonUp(const MouseButton button) const
{
	return details::isMouseButtonValid(button) &&
		m_lastMouseButtonsState.test(button) &&
		!m_currentMouseButtonsState.test(button);
}
