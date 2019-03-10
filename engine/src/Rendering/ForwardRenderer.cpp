#include "Rendering/ForwardRenderer.h"

#include "Managers/RenderingManager.h"

ej::RenderingParameters ej::ForwardRenderer::createRenderingParameters()
{
	RenderingParameters renderingParameters;
	renderingParameters.isDepthTestEnabled = true;
	renderingParameters.isDepthWriteEnabled = true;
	renderingParameters.isBlendingEnabled = false;
	return renderingParameters;
}

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

		const auto& material = entity->getMaterial();
		m_renderingManager->apply(material->getRenderingParameters());
		material->bind();

		auto shader = entity->getMaterial()->getShader();
		m_renderingManager->setCurrentShader(shader);

		shader->setUniform("uCameraViewMatrix", camera->getViewMatrix());
		shader->setUniform("uCameraProjectionMatrix", camera->getProjectionMatrix());
		shader->setUniform("uCameraViewProjectionMatrix", camera->getViewProjectionMatrix());
		shader->setUniform("uCameraRotationMatrix", cameraTransform.getGlobalRotationMatrixInverse());
		shader->setUniform("uCameraPosition", cameraTransform.getGlobalPosition());
		shader->setUniform("uCameraDirection", cameraTransform.getDirectionFront());

		shader->setUniform("uMeshTransformation", meshTransform.getGlobalTransformationMatrix());
		shader->setUniform("uMeshPosition", meshTransform.getGlobalPosition());

		entity->getMesh()->draw();
	}

	m_entities.clear();
}

void ej::ForwardRenderer::setCameraEntity(CameraEntity* entity)
{
	m_cameraEntity = entity;
}

ej::CameraEntity* ej::ForwardRenderer::getCameraEntity() const
{
	return m_cameraEntity;
}
