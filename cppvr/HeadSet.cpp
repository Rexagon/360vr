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

		m_eyeCameras[i] = std::make_unique<ej::Camera>(
			m_vrManager->getEyeProjectionMatrix(static_cast<vr::EVREye>(i), zRange));

		m_eyeTransforms[i].setParent(&m_transform);
		m_eyeTransforms[i].setTransformationMatrix(m_vrManager->getEyeToHeadTransform(static_cast<vr::EVREye>(i)));
	}

	m_screenQuad.init(ej::MeshGeometry::createQuad(glm::vec2(1.0f), 
		ej::MeshGeometry::TEXTURED_VERTEX));

	m_screenQuadShader = core.get<ej::ShaderManager>()->bind("quad",
		ej::ShaderManager::FromFile("shaders/quad.vert"),
		ej::ShaderManager::FromFile("shaders/quad.frag"))->get("quad");

	m_screenQuadShader->setAttribute(0, "vPosition");
	m_screenQuadShader->setAttribute(1, "vTexCoords");

	m_renderingManager->setCurrentShader(m_screenQuadShader.get());
	m_screenQuadShader->setUniform("uLeftEyeTexture", 0);
	m_screenQuadShader->setUniform("uRightEyeTexture", 1);
	m_renderingManager->setCurrentShader(nullptr);
}

void HeadSet::update(const float dt)
{
	if (m_vrManager->isHmdConnected()) {
		m_transform.setPosition(m_vrManager->getHmdPosition());
		m_transform.setRotation(m_vrManager->getHmdRotation());

		for (size_t i = 0; i < 2; ++i) {
			m_eyeCameras[i]->updateView(m_eyeTransforms[i].getGlobalTransformationMatrix());
		}
	}
}

const ej::Camera& HeadSet::bindEye(vr::EVREye eye)
{
	m_renderingManager->setCurrentFrameBuffer(&m_eyeBuffers[eye]);

	const auto& size = m_eyeBuffers[eye].getColorTexture().getSize();
	m_renderingManager->setViewport(0, 0, size.x, size.y);

	return *m_eyeCameras[eye];
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

void HeadSet::drawDebug()
{
	for (unsigned int i = 0; i < 2; ++i) {
		m_eyeBuffers[i].getColorTexture().bind(i);
	}

	m_renderingManager->setCurrentShader(m_screenQuadShader.get());

	m_renderingManager->setClearColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	m_renderingManager->setFaceCullingEnabled(false);
	m_screenQuad.draw();
}

const ej::Transform& HeadSet::getTransform() const
{
	return m_transform;
}

const ej::Transform& HeadSet::getEyeTransform(vr::EVREye eye) const
{
	return m_eyeTransforms[eye];
}
