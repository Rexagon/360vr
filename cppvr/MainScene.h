#pragma once

#include <memory>
#include <mutex>

#include "SceneManager.h"
#include "Skybox.h"

class MainScene : public ej::Scene
{
	enum SkyboxState
	{
		Waiting,
		NeedInitialize,
		Initialized
	} m_skyboxState = Waiting;

public:
	void onInit() override;
	void onClose() override;

	void onUpdate(float dt) override;

private:
	std::unique_ptr<Skybox> m_skybox;
	std::unique_ptr<std::thread> m_streamingThread;

	VideoStream m_videoStream;
	bool m_receivingVideo = false;
};