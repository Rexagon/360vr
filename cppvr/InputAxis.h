#pragma once

#include <vector>

#include <glm/vec2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "PointerDefs.h"

namespace ej
{
	using Key = sf::Keyboard::Key;
	using MouseButton = sf::Mouse::Button;

	class BaseInputAxis
	{
	public:
		virtual ~BaseInputAxis() = default;

		void setSensitivity(float sensitivity);
		float getSensitivity() const;

		void setGravity(float gravity);
		float getGravity() const;

		void setValue(float value);
		float getValue() const;

		void setRange(const glm::vec2& range);
		glm::vec2 getRange() const;

		void setDefaultValue(float value);
		float getDefaultValue() const;

	protected:
		BaseInputAxis();

		float m_sensitivity;
		float m_gravity;
		float m_value;

		glm::vec2 m_range;
		float m_defaultValue;
	};

	template<class T>
	class InputAxis : public BaseInputAxis, public PointerDefs<InputAxis<T>>
	{
	public:
		using KeyMapping = std::pair<T, float>;

		explicit InputAxis(const std::vector<KeyMapping>& mappings);
		virtual ~InputAxis() = default;

		const std::vector<KeyMapping>& getMappings() const;

	protected:
		std::vector<KeyMapping> m_mappings;
	};

	template<class T>
	InputAxis<T>::InputAxis(const std::vector<KeyMapping>& mappings) :
		m_mappings(mappings)
	{}

	template<class T>
	const std::vector<typename InputAxis<T>::KeyMapping>& InputAxis<T>::getMappings() const
	{
		return m_mappings;
	}

	// Type definitions

	using KeyInputAxis = InputAxis<Key>;
	using MouseButtonInputAxis = InputAxis<Key>;
}
