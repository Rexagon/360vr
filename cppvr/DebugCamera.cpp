#include "DebugCamera.h"

DebugCamera::DebugCamera(const ej::Core & core) :
	m_speed(1.0f)
{
	m_inputManager = core.get<ej::InputManager>();
	m_windowManager = core.get<ej::WindowManager>();
}

void DebugCamera::update(const float dt)
{
	const auto windowSize = m_windowManager->getWindow().getSize();
	m_camera.setAspect(static_cast<float>(windowSize.x) / windowSize.y);
	m_camera.updateProjection();

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

	glm::vec2 deltaMouse = m_inputManager->getMousePositionDelta();

	float rotationSpeed = 10.0f;

	const auto hotizontalRotation = glm::angleAxis(-deltaMouse.x * dt * rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
	const auto verticalRotatoin = glm::angleAxis(-deltaMouse.y * dt * rotationSpeed, m_transform.getDirectionRight());

	m_transform.rotate(hotizontalRotation * verticalRotatoin);

	m_transform.move(m_transform.getRotation() * direction * dt * m_speed);	

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

void DebugCamera::setSpeed(float speed)
{
	m_speed = speed;
}

float DebugCamera::getSpeed() const
{
	return m_speed;
}
