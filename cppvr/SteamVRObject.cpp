#include "SteamVRObject.h"

#include <thread>

#include "VRManager.h"
#include "MeshManager.h"
#include "ShaderManager.h"

SteamVRObject::SteamVRObject(const ej::Core& core, const std::string & name) :
	m_isInitialized(false), m_name(name), m_renderModel(nullptr), m_iVRRenderModels(nullptr)
{
	m_meshManager = core.get<ej::MeshManager>();
	m_renderingManager = core.get<ej::RenderingManager>();

	m_iVRRenderModels = core.get<ej::VRManager>()->getRenderModelsInterface();
}

SteamVRObject::~SteamVRObject()
{
	if (m_renderModel) {
		m_iVRRenderModels->FreeRenderModel(m_renderModel);
	}
}

ej::MeshEntity::ptr SteamVRObject::getMeshEntity() const
{
	return m_meshEntity;
}

void SteamVRObject::tryLoad()
{
	if (m_isInitialized) {
		return;
	}

	auto mesh = m_meshManager->get(m_name);

	if (mesh == nullptr) {
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

		mesh = m_meshManager->bind(m_name, [meshGeometry = std::move(meshGeometry)]() {
			return meshGeometry;
		})->get(m_name);

		printf("Loaded SteamVR object: %s\n", m_name.data());
	}

	m_meshEntity->setMesh(mesh);

	m_isInitialized = true;
}
