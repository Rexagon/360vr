#pragma once

#include "Entity.h"
#include "Camera.h"

namespace ej
{
	class CameraEntity : public Entity, public PointerDefs<CameraEntity>
	{
	public:
		CameraEntity(Camera::ptr camera);

		void synchronizeView();

		void setCamera(Camera::ptr camera);
		Camera* getCamera() const;

	private:
		Camera::ptr m_camera;
	};
}