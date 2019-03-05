#pragma once

#include <SFML/Window/Event.hpp>

#include <Core/BaseManager.h>

#include "UI/Widget.h"

class UIManager final : public ej::BaseManager
{
public:
	explicit UIManager(const ej::Core& core);

	void handleEvent(const sf::Event& event);

private:
	std::unique_ptr<ej::Widget> m_rootWidget{};
};
