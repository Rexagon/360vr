#include "UI/Widget.h"

#include <string_view>

Widget::Widget(const ej::Core & core)
{
}

Widget::Widget(const ej::Core& core, std::string_view name) :
	m_name(name)
{
}

void Widget::setName(std::string_view name)
{
	m_name = name;
}

const std::string& Widget::getName() const
{
	return m_name;
}

ej::MeshEntity::ptr Widget::getMeshEntity() const
{
	return m_meshEntity;
}
