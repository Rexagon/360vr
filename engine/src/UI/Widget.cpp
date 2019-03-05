#include "UI/Widget.h"

ej::Widget::Widget(const Core & core)
{
}

ej::Widget::Widget(const ej::Core& core, const std::string& name) :
	m_name(name)
{
}

void ej::Widget::setName(const std::string& name)
{
	m_name = name;
}

const std::string& ej::Widget::getName() const
{
	return m_name;
}

void ej::Widget::setSize(const glm::vec2& size)
{
	m_meshEntity.getTransform().setScale(size.x, size.y, 1.0f);
}

glm::vec2 ej::Widget::getSize() const
{
	return m_meshEntity.getTransform().getScale();
}

void ej::Widget::setOrigin(const glm::vec2& origin)
{
	auto position = m_meshEntity.getTransform().getPosition();
	position.x = origin.x;
	position.y = origin.y;
	m_meshEntity.getTransform().setPosition(position);
}

glm::vec2 ej::Widget::getOrigin() const
{
	return m_meshEntity.getTransform().getPosition();
}

void ej::Widget::setZIndex(const float zIndex)
{
	auto position = m_meshEntity.getTransform().getPosition();
	position.z = zIndex;
	m_meshEntity.getTransform().setPosition(position);
}

float ej::Widget::getZIndex() const
{
	return m_meshEntity.getTransform().getPosition().z;
}

ej::Transform& ej::Widget::getTransform()
{
	return m_transform;
}

ej::MeshEntity* ej::Widget::getMeshEntity()
{
	return &m_meshEntity;
}

void ej::Widget::initMesh(Mesh* mesh, Material* material)
{
	m_meshEntity.setMesh(mesh);
	m_meshEntity.setMaterial(material);
	m_meshEntity.getTransform().setParent(&m_transform);
}
