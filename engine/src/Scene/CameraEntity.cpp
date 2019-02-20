#include "Scene/CameraEntity.h"

ej::CameraEntity::CameraEntity(Camera::ptr camera) :
	m_camera(camera)
{
}

void ej::CameraEntity::synchronizeView()
{
	if (m_camera != nullptr) {
		m_camera->updateView(m_transform.getGlobalTransformationMatrix());
	}
}

void ej::CameraEntity::setCamera(Camera::ptr camera)
{
	m_camera = camera;
}

ej::Camera* ej::CameraEntity::getCamera() const
{
	return m_camera.get();
}
