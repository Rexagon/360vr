#include "Scene/SteamVRObject.h"

#include <thread>

#include <Managers/VRManager.h>
#include <Managers/MeshManager.h>

app::SteamVRObject::SteamVRObject(const ej::Core& core, const std::string & name) :
	m_name(name), m_mesh(core), m_material(core), m_core(core)
{
	m_meshManager = core.get<ej::MeshManager>();
	m_textureManager = core.get<ej::TextureManager>();
	m_renderingManager = core.get<ej::RenderingManager>();

	m_iVRRenderModels = core.get<ej::VRManager>()->getRenderModelsInterface();
}

app::SteamVRObject::~SteamVRObject()
{
	if (m_renderModel) {
		m_iVRRenderModels->FreeRenderModel(m_renderModel);
	}
}

ej::MeshEntity* app::SteamVRObject::getMeshEntity()
{
	ensureLoaded();
	return &m_meshEntity;
}

void app::SteamVRObject::ensureLoaded()
{
	if (m_isInitialized) {
		return;
	}

	if (m_meshEntity.getMesh() == nullptr && !tryLoadMesh()) {
		return;
	}

	if (m_meshEntity.getMaterial() == nullptr && !tryLoadTexture()) {
		return;
	}

	m_isInitialized = true;
}

bool app::SteamVRObject::tryLoadMesh()
{
	//TODO: made caching or move to special manager

	const auto res = m_iVRRenderModels->LoadRenderModel_Async(m_name.data(), &m_renderModel);
	if (res == vr::VRRenderModelError_Loading) {
		return false;
	}

	if (res != vr::VRRenderModelError_None || m_renderModel == nullptr) {
		m_renderModel = nullptr;
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
	m_meshEntity.setMesh(&m_mesh);

	printf("Loaded SteamVR object: %s\n", m_name.data());

	return true;
}

bool app::SteamVRObject::tryLoadTexture()
{
	//TODO: make caching or move to special manager

	if (m_renderModel == nullptr) {
		m_material.setDiffuseTexture(nullptr);
		m_meshEntity.setMaterial(&m_material);
		return true;
	}

	vr::RenderModel_TextureMap_t* diffuseTexture = nullptr;

	const auto res = m_iVRRenderModels->LoadTexture_Async(m_renderModel->diffuseTextureId, &diffuseTexture);
	if (res == vr::VRRenderModelError_Loading) {
		return false;
	}

	if (res != vr::VRRenderModelError_None || diffuseTexture == nullptr) {
		m_material.setDiffuseTexture(nullptr);
		m_meshEntity.setMaterial(&m_material);
		printf("Unable to load SteamVR model: %s\n", m_name.c_str());
		return true;
	}

	m_texture = std::make_unique<ej::Texture>(m_core);
	m_texture->init(diffuseTexture->unWidth, diffuseTexture->unHeight,
			GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, diffuseTexture->rubTextureMapData);

	m_material.setDiffuseTexture(m_texture.get());
	m_meshEntity.setMaterial(&m_material);
	return true;
}
