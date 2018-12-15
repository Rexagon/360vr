#pragma once

#include "Core.h"
#include "Camera.h"
#include "Transform.h"

#include "InputManager.h"
#include "WindowManager.h"

class DebugCamera
{
public:
	DebugCamera(const ej::Core& core);

	void update(const float dt);

	ej::Camera& getCamera();
	ej::Transform& getTransform();

	void setMovementSpeed(float speed);
	float getMovementSpeed() const;

	void setRotationSpeed(float speed);
	float getRotationSpeed() const;

private:
	float m_movementSpeed;
	float m_rotationSpeed;

	ej::InputManager::ptr m_inputManager;
	ej::WindowManager::ptr m_windowManager;

	ej::Camera m_camera;
	ej::Transform m_transform;

	glm::vec2 m_lastMousePosition;
};