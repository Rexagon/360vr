#include "Rendering/UIRenderer.h"

#include "Managers/WindowManager.h"
#include "Managers/RenderingManager.h"

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

	auto state = m_renderingManager->getState();

	state->setDepthTestEnabled(false);
	state->setFaceCullingEnabled(false);
	state->setBlendingEnabled(true);
	state->setBlendingFunctions(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto& entity : m_entities) {
		if (entity->getMesh() == nullptr || entity->getMaterial() == nullptr ||
			entity->getMaterial()->getShader() == nullptr) 
		{
			continue;
		}

		const auto& meshTransform = entity->getTransform();

		entity->getMaterial()->bind();

		auto shader = entity->getMaterial()->getShader();
		state->setCurrentShader(shader);

		shader->setUniform("uCameraViewProjectionMatrix", m_camera.getViewProjectionMatrix());
		shader->setUniform("uMeshTransformation", meshTransform.getGlobalTransformationMatrix());
		shader->setUniform("uWindowSize", glm::vec2(windowSize.x, windowSize.y));

		entity->getMesh()->draw();
	}

	m_entities.clear();
}
