#pragma once

#include <SFML/Window/Event.hpp>

#include <Core/BaseManager.h>

#include "UI/Widget.h"

class UIManager : public ej::BaseManager, public ej::PointerDefs<UIManager>
{
public:
	explicit UIManager(const ej::Core& core);

	void handleEvent(const sf::Event& event);

private:
	std::shared_ptr<Widget> m_rootWidget;
};
