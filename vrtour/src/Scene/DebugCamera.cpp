#include "Scene/DebugCamera.h"

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
	const auto windowCenter = glm::ivec2(windowSize.x / 2, windowSize.y / 2);

	if (m_inputManager->getMouseButtonDown(ej::MouseButton::Right)) {
		m_lastMousePosition = m_inputManager->getMousePosition();
		m_inputManager->setMousePosition(windowCenter);
		m_inputManager->setMouseCursorVisible(false);
	}

	if (m_inputManager->getMouseButtonUp(ej::MouseButton::Right)) {
		m_inputManager->setMousePosition(m_lastMousePosition);

		m_inputManager->setMouseCursorVisible(true);
	}

	if (m_inputManager->getMouseButton(ej::MouseButton::Right)) {
		const auto deltaMouse = m_inputManager->getMousePosition() - windowCenter;

		const auto horizontalRotation = angleAxis(-deltaMouse.x * dt * m_rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
		transform.rotate(horizontalRotation);

		const auto verticalRotation = angleAxis(-deltaMouse.y * dt * m_rotationSpeed, transform.getDirectionRight());
		transform.rotate(verticalRotation);

		m_inputManager->setMousePosition(windowCenter);
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
