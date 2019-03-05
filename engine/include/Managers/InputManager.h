#pragma once

#include <bitset>

#include <SFML/Window/Event.hpp>

#include "Core/BaseManager.h"
#include "Managers/WindowManager.h"

namespace ej
{
	using Key = sf::Keyboard::Key;
	using MouseButton = sf::Mouse::Button;

	namespace details
	{
		/**
		 * \brief Check if key is in valid range
		 * \param key Keyboard key
		 * \return true if is valid
		 */
		bool isKeyValid(Key key);
		
		/**
		 * \brief Check if button is in valid range
		 * \param button Mouse button
		 * \return true if is valid
		 */
		bool isMouseButtonValid(MouseButton button);
	}

	/**
	 * \brief Give access to the real-time state of input
	 */
	class InputManager final : public BaseManager
	{
		using KeysState = std::bitset<Key::KeyCount>;
		using MouseButtonsState = std::bitset<MouseButton::ButtonCount>;

	public:
		/**
		 * \param core Owner of this manager
		 */
		explicit InputManager(const Core& core);

		/**
		 * \brief Update input state
		 * 
		 * Mark current state as previous. Reset some values.
		 * Must be called every frame before InputManager::handleEvent() 
		 * is called.
		 */
		void updateState();

		/**
		 * \brief Handle window event
		 * 
		 * Handles key/button press, mouse movement and mouse scroll.
		 * Should be used only after InputManager::updateState() on
		 * same frame.
		 * 
		 * \param event Window event
		 */
		void handleEvent(const sf::Event& event);

		/**
		 * \brief Get key state
		 * \param key Key code
		 * \return true if pressed
		 */
		bool getKey(Key key) const;

		/**
		 * \brief Check if key was pressed since last frame
		 * \param key Key code
		 * \return true if pressed
		 */
		bool getKeyDown(Key key) const;

		/**
		 * \brief Check if key was released since last frame
		 * \param key Key code
		 * \return true if released
		 */
		bool getKeyUp(Key key) const;

		/**
		 * \brief Check if any key is pressed
		 * \return true if any key is pressed
		 */
		bool getAnyKey() const;

		/**
		 * \brief Check if any key was pressed since last frame
		 * \return true if pressed
		 */
		bool getAnyKeyDown() const;

		/**
		 * \brief Check if any key was released since last frame
		 * \return true if released
		 */
		bool getAnyKeyUp() const;

		/**
		 * \brief Check mouse button state
		 * \param button Mouse button code
		 * \return true if pressed
		 */
		bool getMouseButton(MouseButton button) const;

		/**
		 * \brief Check if mouse button was pressed since last frame
		 * \param button Mouse button code
		 * \return true if pressed
		 */
		bool getMouseButtonDown(MouseButton button) const;

		/**
		 * \brief Check if mouse button was released since last frame
		 * \param button Mouse button code
		 * \return true if released
		 */
		bool getMouseButtonUp(MouseButton button) const;

		/**
		 * \brief Set cursor visibility
		 * \param visible Cursor visibility. Show on true, hide otherwise
		 */
		void setMouseCursorVisible(bool visible);

		/**
		 * \brief Set mouse position
		 * \param position Mouse position relative to current window
		 */
		void setMousePosition(const glm::ivec2& position);

		/**
		 * \brief Get mouse delta position
		 * \return mouse delta position since last frame
		 */
		glm::ivec2 getMousePositionDelta() const;

		/**
		 * \brief Get mouse position
		 * \return mouse position relative to current window
		 */
		glm::ivec2 getMousePosition() const;

	private:
		WindowManager* m_windowManager{ nullptr };

		bool m_anyKeyUp{ false };
		bool m_anyKeyDown{ false };
		KeysState m_currentKeysState;
		KeysState m_lastKeysState;

		bool m_anyMouseButtonUp{ false };
		bool m_anyMouseButtonDown{ false };
		MouseButtonsState m_currentMouseButtonsState;
		MouseButtonsState m_lastMouseButtonsState;

		glm::ivec2 m_lastMousePosition{};
		glm::ivec2 m_currentMousePosition{};

		float m_mouseWheelDelta{ 0.0f };
	};
}
