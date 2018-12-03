#include "InputAxis.h"

#include <glm/common.hpp>

using namespace ej;

BaseInputAxis::BaseInputAxis() :
	m_sensitivity(1.0f),
	m_gravity(1.0f),
	m_value(0.0f),
	m_range(-1.0f, 1.0f),
	m_defaultValue(0.0f)
{
}

void BaseInputAxis::setSensitivity(float sensitivity)
{
	m_sensitivity = sensitivity;
}

float BaseInputAxis::getSensitivity() const
{
	return m_sensitivity;
}

void BaseInputAxis::setGravity(float gravity)
{
	m_gravity = gravity;
}

float BaseInputAxis::getGravity() const
{
	return m_gravity;
}

void BaseInputAxis::setValue(float value)
{
	m_value = value;
}

float BaseInputAxis::getValue() const
{
	return m_value;
}

void BaseInputAxis::setRange(const glm::vec2 & range)
{
	m_range = range;
	m_value = glm::clamp(m_value, range.x, range.y);
}

glm::vec2 BaseInputAxis::getRange() const
{
	return m_range;
}

void BaseInputAxis::setDefaultValue(float value)
{
	m_defaultValue = value;
}

float BaseInputAxis::getDefaultValue() const
{
	return m_defaultValue;
}