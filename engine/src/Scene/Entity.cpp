#include "Scene/Entity.h"

void ej::Entity::setName(const std::string& name)
{
	m_name = name;
}

const std::string& ej::Entity::getName() const
{
	return m_name;
}

void ej::Entity::setTransform(const Transform& transform)
{
	m_transform = transform;
}

ej::Transform& ej::Entity::getTransform()
{
	return m_transform;
}
