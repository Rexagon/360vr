#pragma once

#include <Scene/CameraEntity.h>
#include <Managers/InputManager.h>
#include <Managers/WindowManager.h>

namespace app
{
	class DebugCamera final
	{
	public:
		explicit DebugCamera(const ej::Core& core);

		void update(float dt);

		ej::CameraEntity* getCameraEntity();

		void setMovementSpeed(float speed);
		float getMovementSpeed() const;

		void setRotationSpeed(float speed);
		float getRotationSpeed() const;

	private:
		ej::InputManager* m_inputManager = nullptr;
		ej::WindowManager* m_windowManager = nullptr;

		float m_movementSpeed = 1.0f;
		float m_rotationSpeed = 1.0f;

		glm::vec2 m_rotation{ 0.0f };

		ej::Camera m_camera{};
		ej::CameraEntity m_cameraEntity;

		glm::vec2 m_lastMousePosition{ 0.0f };
	};
}
