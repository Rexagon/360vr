#include "UI/Widget.h"

ej::Widget::Widget(const Core & core)
{
}

ej::Widget::Widget(const ej::Core& core, const std::string& name) :
	m_name(name)
{
}

void ej::Widget::setName(std::string_view name)
{
	m_name = name;
}

const std::string& ej::Widget::getName() const
{
	return m_name;
}

void ej::Widget::setSize(const glm::vec2& size)
{
	if (m_meshEntity != nullptr) {
		m_meshEntity->getTransform().setScale(size.x, size.y, 1.0f);
	}
}

glm::vec2 ej::Widget::getSize() const
{
	if (m_meshEntity == nullptr) {
		return glm::vec2(0.0f, 0.0f);
	}

	return m_meshEntity->getTransform().getScale();
}

void ej::Widget::setOrigin(const glm::vec2& origin)
{
	if (m_meshEntity != nullptr) {
		auto position = m_meshEntity->getTransform().getPosition();
		position.x = origin.x;
		position.y = origin.y;
		m_meshEntity->getTransform().setPosition(position);
	}
}

glm::vec2 ej::Widget::getOrigin() const
{
	if (m_meshEntity == nullptr) {
		return glm::vec2(0.0f, 0.0f);
	}

	return m_meshEntity->getTransform().getPosition();
}

void ej::Widget::setZIndex(float zIndex)
{
	auto position = m_meshEntity->getTransform().getPosition();
	position.z = zIndex;
	m_meshEntity->getTransform().setPosition(position);
}

float ej::Widget::getZIndex() const
{
	if (m_meshEntity == nullptr) {
		return 0.0f;
	}

	return m_meshEntity->getTransform().getPosition().z;
}

ej::Transform& ej::Widget::getTransform()
{
	return m_transform;
}

ej::MeshEntity::ptr ej::Widget::getMeshEntity() const
{
	return m_meshEntity;
}

void ej::Widget::initMesh(Mesh::ptr mesh, std::shared_ptr<Material> material)
{
	m_meshEntity = std::make_shared<ej::MeshEntity>(mesh, material);
	m_meshEntity->getTransform().setParent(&m_transform);
}
