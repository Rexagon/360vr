#include "InputManager.h"

using namespace core;

bool details::isKeyValid(Key key)
{
	return key > Key::Unknown && key < Key::KeyCount;
}

bool details::isMouseButtonValid(MouseButton button)
{
	return button > -1 && button < MouseButton::ButtonCount;
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

bool InputManager::getKey(Key key) const
{
	return details::isKeyValid(key) &&
		m_currentKeysState.test(static_cast<size_t>(key));
}

bool InputManager::getKeyDown(Key key) const
{
	return details::isKeyValid(key) &&
		!m_lastKeysState.test(key) &&
		m_currentKeysState.test(key);
}

bool InputManager::getKeyUp(Key key) const
{
	return details::isKeyValid(key) &&
		m_lastKeysState.test(key) &&
		!m_currentKeysState.test(key);
}

bool InputManager::getMouseButton(MouseButton button) const
{
	return details::isMouseButtonValid(button) &&
		m_currentMouseButtonsState.test(button);
}

bool InputManager::getMouseButtonDown(MouseButton button) const
{
	return details::isMouseButtonValid(button) &&
		!m_lastMouseButtonsState.test(button) &&
		m_currentMouseButtonsState.test(button);
}

bool InputManager::getMouseButtonUp(MouseButton button) const
{
	return details::isMouseButtonValid(button) &&
		m_lastMouseButtonsState.test(button) &&
		!m_currentMouseButtonsState.test(button);
}
