#include "HeadSet.h"

#include "ShaderManager.h"

HeadSet::HeadSet(const ej::Core & core)
{
	m_vrManager = core.get<ej::VRManager>();
	m_renderingManager = core.get<ej::RenderingManager>();

	const glm::vec2 zRange(0.1f, 100.0f);
	const auto renderTargetSize = m_vrManager->getRenderTargetSize();

	for (size_t i = 0; i < 2; ++i) {
		m_eyeBuffers[i].init(renderTargetSize.x, renderTargetSize.y, true);

		auto camera = std::make_shared<ej::Camera>(
			m_vrManager->getEyeProjectionMatrix(static_cast<vr::EVREye>(i), zRange));

		auto cameraEntity = std::make_shared<ej::CameraEntity>(camera);
		auto& transform = cameraEntity->getTransform();

		transform.setParent(&m_transform);
		transform.setTransformationMatrix(m_vrManager->getEyeToHeadTransform(static_cast<vr::EVREye>(i)));

		m_cameraEntities[i] = cameraEntity;
	}
}

void HeadSet::update(const float dt)
{
	if (m_vrManager->isHmdConnected()) {
		m_transform.setPosition(m_vrManager->getHmdPosition());
		m_transform.setRotation(m_vrManager->getHmdRotation());

		for (size_t i = 0; i < 2; ++i) {
			m_cameraEntities[i]->synchronizeView();
		}
	}
}

void HeadSet::bindEye(vr::EVREye eye)
{
	auto state = m_renderingManager->getState();

	state->setCurrentFrameBuffer(&m_eyeBuffers[eye]);

	const auto& size = m_eyeBuffers[eye].getColorTexture().getSize();
	state->setViewport(0, 0, size.x, size.y);
}

void HeadSet::submit()
{
	static vr::Texture_t leftEyeTexture = {
		reinterpret_cast<void*>(static_cast<uintptr_t>(m_eyeBuffers[0].getColorTexture().getHandle())),
		vr::TextureType_OpenGL,
		vr::ColorSpace_Gamma
	};

	static vr::Texture_t rightEyeBuffer = {
		reinterpret_cast<void*>(static_cast<uintptr_t>(m_eyeBuffers[1].getColorTexture().getHandle())),
		vr::TextureType_OpenGL,
		vr::ColorSpace_Gamma
	};

	m_vrManager->getCompositorInterface()->Submit(vr::Eye_Left, &leftEyeTexture);
	m_vrManager->getCompositorInterface()->Submit(vr::Eye_Right, &rightEyeBuffer);
	glFlush();
}

const ej::Transform& HeadSet::getTransform() const
{
	return m_transform;
}

ej::CameraEntity::ptr HeadSet::getCameraEntity(vr::EVREye eye) const
{
	return m_cameraEntities[static_cast<size_t>(eye)];
}
