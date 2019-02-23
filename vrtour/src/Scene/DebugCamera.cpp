#include "Scene/DebugCamera.h"

#include <cmath>
#include <Core/Core.h>

DebugCamera::DebugCamera(const ej::Core & core) :
	m_movementSpeed(1.0f), m_rotationSpeed(1.0f)
{
	m_inputManager = core.get<ej::InputManager>();
	m_windowManager = core.get<ej::WindowManager>();

	auto camera = std::make_shared<ej::Camera>();
	m_cameraEntity = std::make_shared<ej::CameraEntity>(camera);
}

void DebugCamera::update(const float dt)
{
	auto camera = m_cameraEntity->getCamera();
	auto& transform = m_cameraEntity->getTransform();

	const auto windowSize = m_windowManager->getWindow().getSize();
	m_cameraEntity->getCamera()->setAspect(static_cast<float>(windowSize.x) / windowSize.y);
	m_cameraEntity->getCamera()->updateProjection();
	
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

	if (m_inputManager->getKey(ej::Key::LControl)) {
		direction.y -= 1.0f;
	}
	else if (m_inputManager->getKey(ej::Key::Space)) {
		direction.y += 1.0f;
	}

	transform.move(transform.getRotation() * direction * dt * m_movementSpeed);

	// Update view
	m_cameraEntity->synchronizeView();
}

ej::CameraEntity::ptr DebugCamera::getCameraEntity()
{
	return m_cameraEntity;
}

void DebugCamera::setMovementSpeed(float speed)
{
	m_movementSpeed = speed;
}

float DebugCamera::getMovementSpeed() const
{
	return m_movementSpeed;
}

void DebugCamera::setRotationSpeed(float speed)
{
	m_rotationSpeed = speed;
}

float DebugCamera::getRotationSpeed() const
{
	return m_rotationSpeed;
}
