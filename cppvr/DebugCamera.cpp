#include "DebugCamera.h"

#include "Core.h"

DebugCamera::DebugCamera(const ej::Core & core) :
	m_movementSpeed(1.0f), m_rotationSpeed(1.0f)
{
	m_inputManager = core.get<ej::InputManager>();
	m_windowManager = core.get<ej::WindowManager>();
}

void DebugCamera::update(const float dt)
{
	const auto windowSize = m_windowManager->getWindow().getSize();
	m_camera.setAspect(static_cast<float>(windowSize.x) / windowSize.y);
	m_camera.updateProjection();
	
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
		glm::vec2 deltaMouse = m_inputManager->getMousePosition() - windowCenter;

		const auto horizontalRotation = glm::angleAxis(-deltaMouse.x * dt * m_rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
		m_transform.rotate(horizontalRotation);

		const auto verticalRotation = glm::angleAxis(-deltaMouse.y * dt * m_rotationSpeed, m_transform.getDirectionRight());
		m_transform.rotate(verticalRotation);

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

	m_transform.move(m_transform.getRotation() * direction * dt * m_movementSpeed);	

	// Update view

	m_camera.updateView(m_transform.getGlobalTransformationMatrix());
}

ej::Camera & DebugCamera::getCamera()
{
	return m_camera;
}

ej::Transform & DebugCamera::getTransform()
{
	return m_transform;
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
