#include "Scene/HeadSet.h"

HeadSet::HeadSet(const ej::Core & core) :
	m_cameraEntities{ ej::CameraEntity(&m_cameras[0]), ej::CameraEntity(&m_cameras[1]) },
	m_eyeBuffers{ ej::FrameBuffer(core), ej::FrameBuffer(core) }
{
	m_vrManager = core.get<ej::VRManager>();
	m_renderingManager = core.get<ej::RenderingManager>();

	const glm::vec2 zRange(0.1f, 100.0f);
	const auto renderTargetSize = m_vrManager->getRenderTargetSize();

	for (size_t i = 0; i < 2; ++i) {
		m_eyeBuffers[i].init(renderTargetSize.x, renderTargetSize.y, true);

		m_cameras[i].setCustomProjection(
			m_vrManager->getEyeProjectionMatrix(static_cast<vr::EVREye>(i), zRange)
		);

		auto& transform = m_cameraEntities[i].getTransform();
		transform.setParent(&m_transform);
		transform.setTransformationMatrix(m_vrManager->getEyeToHeadTransform(static_cast<vr::EVREye>(i)));
	}
}

void HeadSet::update(const float dt)
{
	if (m_vrManager->isHmdConnected()) {
		m_transform.setPosition(m_vrManager->getHmdPosition());
		m_transform.setRotation(m_vrManager->getHmdRotation());

		for (auto& cameraEntity : m_cameraEntities) {
			cameraEntity.synchronizeView();
		}
	}
}

void HeadSet::bindEye(const vr::EVREye eye)
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

ej::CameraEntity* HeadSet::getCameraEntity(const vr::EVREye eye)
{
	return &m_cameraEntities[static_cast<size_t>(eye)];
}
