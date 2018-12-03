#include "SteamVRObject.h"

#include <thread>

#include "VRManager.h"
#include "ShaderManager.h"

SteamVRObject::SteamVRObject(const ej::Core& core, const std::string & name) :
	m_isInitialized(false), m_name(name), m_renderModel(nullptr), m_iVRRenderModels(nullptr)
{
	m_iVRRenderModels = core.get<ej::VRManager>()->getRenderModelsInterface();

	m_shader = core.get<ej::ShaderManager>()->bind("steamvr",
		ej::ShaderManager::FromFile("shaders/steamvr.vert"),
		ej::ShaderManager::FromFile("shaders/steamvr.frag"))->get("steamvr");

	m_shader->setAttribute(0, "vPosition");
	m_shader->setAttribute(1, "vTexCoords");
	m_shader->setAttribute(2, "vNormal");

	glUseProgram(m_shader->getHandle());
	m_shader->setUniform("uViewProjection", glm::mat4());
	m_shader->setUniform("uDiffuseTexture", 0);
	m_shader->setUniform("uSkyboxTexture", 3);
	glUseProgram(0);
}

SteamVRObject::~SteamVRObject()
{
	if (m_renderModel) {
		m_iVRRenderModels->FreeRenderModel(m_renderModel);
	}
}

void SteamVRObject::draw(const ej::Camera& camera, const ej::Transform& cameraTransform, const ej::Transform& transform)
{
	draw(camera, cameraTransform, transform.getTransformationMatrix());
}

void SteamVRObject::draw(const ej::Camera& camera, const ej::Transform& cameraTransform, const glm::mat4& transform)
{
	if (!m_isInitialized) {
		tryLoad();
		return;
	}

	glCullFace(GL_BACK);

	glUseProgram(m_shader->getHandle());
	m_shader->setUniform("uViewProjection", camera.getViewProjectionMatrix());
	m_shader->setUniform("uTransform", transform);
	m_shader->setUniform("uCameraPosition", cameraTransform.getGlobalPosition());

	m_mesh.draw();
	glUseProgram(0);
}

void SteamVRObject::tryLoad()
{
	if (m_isInitialized) {
		return;
	}

	// "{system}generic_controller"
	vr::EVRRenderModelError res;
	if ((res = m_iVRRenderModels->LoadRenderModel_Async(m_name.data(), &m_renderModel)) == vr::VRRenderModelError_Loading) {
		return;
	}

	if (res != vr::VRRenderModelError_None || m_renderModel == nullptr) {
		throw std::runtime_error("Unable to load SteamVR model: " + m_name);
	}

	ej::MeshGeometry meshGeometry;

	meshGeometry.vertexComponents = ej::MeshGeometry::MODEL_VERTEX;

	const auto vertexCount = m_renderModel->unVertexCount;

	meshGeometry.positions.resize(vertexCount);
	meshGeometry.texCoords.resize(vertexCount);
	meshGeometry.normals.resize(vertexCount);

	for (uint32_t i = 0; i < vertexCount; ++i) {
		std::memcpy(&meshGeometry.positions[i][0], 
			m_renderModel->rVertexData[i].vPosition.v, 3 * sizeof(float));
		std::memcpy(&meshGeometry.texCoords[i][0], 
			m_renderModel->rVertexData[i].rfTextureCoord, 2 * sizeof(float));
		std::memcpy(&meshGeometry.normals[i][0], 
			m_renderModel->rVertexData[i].vNormal.v, 3 * sizeof(float));
	}

	const auto indexCount = m_renderModel->unTriangleCount * 3;

	meshGeometry.indices.resize(indexCount);

	for (uint32_t i = 0; i < indexCount; ++i) {
		meshGeometry.indices[i] = m_renderModel->rIndexData[i];
	}

	m_mesh.init(meshGeometry);

	printf("Loaded SteamVR object: %s\n", m_name.data());

	m_isInitialized = true;
}
