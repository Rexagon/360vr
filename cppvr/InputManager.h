#pragma once

#include <map>
#include <bitset>

#include <SFML/Window/Event.hpp>

#include "InputAxis.h"
#include "BaseManager.h"
#include "WindowManager.h"

namespace ej
{
	using Key = sf::Keyboard::Key;
	using MouseButton = sf::Mouse::Button;

	namespace details
	{
		bool isKeyValid(Key key);
		bool isMouseButtonValid(MouseButton button);
	}

	class InputManager : public BaseManager, public PointerDefs<InputManager>
	{
		using KeysState = std::bitset<Key::KeyCount>;
		using MouseButtonsState = std::bitset<MouseButton::ButtonCount>;

	public:
		explicit InputManager(const Core& core);

		void updateState();
		void handleEvent(const sf::Event& event);

		bool getKey(Key key) const;
		bool getKeyDown(Key key) const;
		bool getKeyUp(Key key) const;

		bool getAnyKey() const;
		bool getAnyKeyDown() const;
		bool getAnyKeyUp() const;

		bool getMouseButton(MouseButton button) const;
		bool getMouseButtonDown(MouseButton button) const;
		bool getMouseButtonUp(MouseButton button) const;

		float getAxis(const std::string& name);

		void setMouseCursorVisible(bool visible);

		void setMousePosition(const glm::ivec2& position);
		glm::ivec2 getMousePositionDelta() const;
		glm::ivec2 getMousePosition() const;

	private:
		ej::WindowManager::ptr m_windowManager;

		bool m_anyKeyUp;
		bool m_anyKeyDown;
		KeysState m_currentKeysState;
		KeysState m_lastKeysState;

		bool m_anyMouseButtonUp;
		bool m_anyMouseButtonDown;
		MouseButtonsState m_currentMouseButtonsState;
		MouseButtonsState m_lastMouseButtonsState;

		glm::ivec2 m_lastMousePosition;
		glm::ivec2 m_currentMousePosition;

		float m_mouseWheelDelta;

		std::map<std::string, std::shared_ptr<BaseInputAxis>> m_axes;
	};
}
