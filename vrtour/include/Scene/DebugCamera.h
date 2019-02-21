#pragma once

#include <Scene/CameraEntity.h>
#include <Managers/InputManager.h>
#include <Managers/WindowManager.h>

class DebugCamera : public ej::PointerDefs<DebugCamera>
{
public:
	DebugCamera(const ej::Core& core);

	void update(float dt);

	ej::CameraEntity::ptr getCameraEntity();

	void setMovementSpeed(float speed);
	float getMovementSpeed() const;

	void setRotationSpeed(float speed);
	float getRotationSpeed() const;

private:
	float m_movementSpeed;
	float m_rotationSpeed;

	glm::vec2 m_rotation;

	ej::InputManager::ptr m_inputManager;
	ej::WindowManager::ptr m_windowManager;

	ej::CameraEntity::ptr m_cameraEntity;

	glm::vec2 m_lastMousePosition;
};
