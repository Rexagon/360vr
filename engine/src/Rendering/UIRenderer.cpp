#include "Rendering/UIRenderer.h"

#include "Managers/WindowManager.h"
#include "Managers/RenderingManager.h"

ej::RenderingParameters ej::UIRenderer::createRenderingParameters()
{
	RenderingParameters renderingParameters;
	renderingParameters.isDepthTestEnabled = false;
	renderingParameters.isFaceCullingEnabled = false;
	renderingParameters.isBlendingEnabled = true;
	renderingParameters.blendingFunctionSrc = GL_SRC_ALPHA;
	renderingParameters.blendingFunctionDst = GL_ONE_MINUS_SRC_ALPHA;
	return renderingParameters;
}

ej::UIRenderer::UIRenderer(const Core& core) :
	Renderer(core), m_cameraEntity(&m_camera)
{
	m_windowManager = core.get<WindowManager>();

	m_camera.setProjectionType(Camera::Isometric);
	m_camera.setDepthRange(glm::vec2(-100.0f, 100.0f));
}

void ej::UIRenderer::draw()
{
	const auto windowSize = sf::Vector2f(m_windowManager->getWindow().getSize());

	m_camera.setDimensions(windowSize.x * 0.5f, windowSize.y * 0.5f);
	m_camera.updateProjection();

	m_cameraEntity.synchronizeView();

	for (auto& entity : m_entities) {
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

		shader->setUniform("uCameraViewProjectionMatrix", m_camera.getViewProjectionMatrix());
		shader->setUniform("uMeshTransformation", meshTransform.getGlobalTransformationMatrix());
		shader->setUniform("uWindowSize", glm::vec2(windowSize.x, windowSize.y));

		entity->getMesh()->draw();
	}

	m_entities.clear();
}
