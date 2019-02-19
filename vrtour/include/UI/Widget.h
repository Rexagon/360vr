#pragma once

#include <SFML/Window/Event.hpp>

#include <Scene/MeshEntity.h>

class Widget
{
public:
	Widget(const ej::Core& core);
	Widget(const ej::Core& core, std::string_view name);

	virtual void update(float dt) = 0;

	virtual void handleEvent(const sf::Event& event) {};

	void setName(std::string_view name);
	const std::string& getName() const;

protected:
	std::string m_name;
	ej::MeshEntity::ptr m_meshEntity;
};
