#include "Rendering/ForwardRenderer.h"

#include "Managers/RenderingManager.h"

ej::ForwardRenderer::ForwardRenderer(const Core& core) :
	Renderer(core)
{
}

void ej::ForwardRenderer::draw()
{
	if (m_cameraEntity == nullptr || m_cameraEntity->getCamera() == nullptr) {
		m_entities.clear();
		return;
	}

	const auto& camera = m_cameraEntity->getCamera();
	const auto& cameraTransform = m_cameraEntity->getTransform();

	for (const auto& entity : m_entities) {
		if (entity->getMesh() == nullptr || entity->getMaterial() == nullptr ||
			entity->getMaterial()->getShader() == nullptr) 
		{
			continue;
		}

		const auto& meshTransform = entity->getTransform();

		entity->getMaterial()->bind();

		auto shader = entity->getMaterial()->getShader();
		m_renderingManager->getState()->setCurrentShader(shader);

		shader->setUniform("uCameraViewMatrix", camera->getViewMatrix());
		shader->setUniform("uCameraProjectionMatrix", camera->getProjectionMatrix());
		shader->setUniform("uCameraViewProjectionMatrix", camera->getViewProjectionMatrix());
		shader->setUniform("uCameraRotationMatrix", cameraTransform.getRotationMatrixInverse());
		shader->setUniform("uCameraPosition", cameraTransform.getGlobalPosition());

		shader->setUniform("uMeshTransformation", meshTransform.getGlobalTransformationMatrix());
		shader->setUniform("uMeshPosition", meshTransform.getGlobalPosition());

		entity->getMesh()->draw();
	}

	m_entities.clear();
}

void ej::ForwardRenderer::setCameraEntity(CameraEntity::ptr entity)
{
	m_cameraEntity = entity;
}

ej::CameraEntity* ej::ForwardRenderer::getCameraEntity() const
{
	return m_cameraEntity.get();
}
