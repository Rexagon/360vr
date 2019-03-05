#pragma once

#include "Entity.h"
#include "Camera.h"

namespace ej
{
	class CameraEntity final : public Entity
	{
	public:
		explicit CameraEntity(Camera* camera);

		void synchronizeView();

		void setCamera(Camera* camera);
		Camera* getCamera() const;

	private:
		Camera* m_camera{ nullptr };
	};
}