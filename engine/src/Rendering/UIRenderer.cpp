#include "Rendering/UIRenderer.h"

#include "Managers/WindowManager.h"
#include "Managers/RenderingManager.h"

ej::UIRenderer::UIRenderer(const Core& core) :
	Renderer(core)
{
	m_windowManager = core.get<WindowManager>();

	auto camera = std::make_shared<Camera>();
	camera->setProjectionType(Camera::Isometric);

	m_cameraEntity = std::make_shared<CameraEntity>(camera);
}

void ej::UIRenderer::draw()
{
	const auto& camera = m_cameraEntity->getCamera();
	const auto& cameraTransform = m_cameraEntity->getTransform();

	const auto windowSize = sf::Vector2f(m_windowManager->getWindow().getSize());

	camera->setDepthRange(glm::vec2(-100.0f, 100.0f));
	camera->setDimensions(windowSize.x * 0.5f, windowSize.y * 0.5);
	camera->updateProjection();

	m_cameraEntity->synchronizeView();

	auto state = m_renderingManager->getState();

	state->setDepthTestEnabled(false);
	state->setFaceCullingEnabled(false);

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

		shader->setUniform("uCameraViewProjectionMatrix", camera->getViewProjectionMatrix());
		shader->setUniform("uMeshTransformation", meshTransform.getGlobalTransformationMatrix());
		shader->setUniform("uWindowSize", glm::vec2(windowSize.x, windowSize.y));

		entity->getMesh()->draw();
	}

	m_entities.clear();
}
