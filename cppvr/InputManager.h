#pragma once

#include <bitset>
#include <SFML/Window/Event.hpp>

#include "BaseManager.h"

namespace core
{

using Key = sf::Keyboard::Key;
using MouseButton = sf::Mouse::Button;

namespace details
{
	bool isKeyValid(Key key);
	bool isMouseButtonValid(MouseButton button);
}

class InputManager : public BaseManager
{
	using KeysState = std::bitset<Key::KeyCount>;
	using MouseButtonsState = std::bitset<MouseButton::ButtonCount>;

public:
	void updateState();
	void handleEvent(const sf::Event& event);

	bool getKey(Key key) const;
	bool getKeyDown(Key key) const;
	bool getKeyUp(Key key) const;

	bool getMouseButton(MouseButton button) const;
	bool getMouseButtonDown(MouseButton button) const;
	bool getMouseButtonUp(MouseButton button) const;

private:
	KeysState m_currentKeysState;
	KeysState m_lastKeysState;

	MouseButtonsState m_currentMouseButtonsState;
	MouseButtonsState m_lastMouseButtonsState;
};

}