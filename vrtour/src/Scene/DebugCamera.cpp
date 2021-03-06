#include "Scene/DebugCamera.h"

#include <cmath>
#include <Core/Core.h>

app::DebugCamera::DebugCamera(const ej::Core & core) :
	m_cameraEntity(&m_camera)
{
	m_inputManager = core.get<ej::InputManager>();
	m_windowManager = core.get<ej::WindowManager>();
}

void app::DebugCamera::update(const float dt)
{
	auto& transform = m_cameraEntity.getTransform();

	const auto windowSize = m_windowManager->getWindow().getSize();
	m_camera.setAspect(static_cast<float>(windowSize.x) / windowSize.y);
	m_camera.updateProjection();
	
	// Handle rotation

	if (m_inputManager->getMouseButton(ej::MouseButton::Right)) {
		const auto deltaMouse = glm::vec2(m_inputManager->getMousePositionDelta());

		m_rotation -= deltaMouse * m_rotationSpeed;
		m_rotation.x = std::fmod(m_rotation.x, 360.0f);
		m_rotation.y = glm::clamp(m_rotation.y, -80.0f, 80.0f);

		transform.setRotation(0.0f, m_rotation.x, 0.0f);
		transform.rotate(angleAxis(glm::radians(m_rotation.y), transform.getDirectionRight()));
	}

	// Handle movement
	glm::vec3 direction{ 0.0f, 0.0f, 0.0f };

	if (m_inputManager->getKey(ej::Key::W)) {
		direction.z -= 1.0f;
	}
	else if (m_inputManager->getKey(ej::Key::S)) {
		direction.z += 1.0f;
	}

	if (m_inputManager->getKey(ej::Key::A)) {
		direction.x -= 1.0f;
	}
	else if (m_inputManager->getKey(ej::Key::D)) {
		direction.x += 1.0f;
	}

	if (m_inputManager->getKey(ej::Key::C)) {
		direction.y -= 1.0f;
	}
	else if (m_inputManager->getKey(ej::Key::Space)) {
		direction.y += 1.0f;
	}

	transform.move(transform.getRotation() * direction * dt * m_movementSpeed);

	// Update view
	m_cameraEntity.synchronizeView();
}

ej::CameraEntity* app::DebugCamera::getCameraEntity()
{
	return &m_cameraEntity;
}

void app::DebugCamera::setMovementSpeed(const float speed)
{
	m_movementSpeed = speed;
}

float app::DebugCamera::getMovementSpeed() const
{
	return m_movementSpeed;
}

void app::DebugCamera::setRotationSpeed(const float speed)
{
	m_rotationSpeed = speed;
}

float app::DebugCamera::getRotationSpeed() const
{
	return m_rotationSpeed;
}
