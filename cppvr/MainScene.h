#pragma once

#include "SceneManager.h"

class MainScene : public ej::Scene
{
public:
	void onInit() override;
	void onClose() override;

	void onUpdate(const float dt) override;

private:

};