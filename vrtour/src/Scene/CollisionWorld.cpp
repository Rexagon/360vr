#include "Scene/CollisionWorld.h"

#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"

app::CollisionWorld::CollisionWorld()
{
	m_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();

	m_collisionDispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get());
	m_broadphase = std::make_unique<btDbvtBroadphase>();

	m_collisionWorld = std::make_unique<btCollisionWorld>(m_collisionDispatcher.get(), 
		m_broadphase.get(), m_collisionConfiguration.get());
}

bool app::CollisionWorld::raycast(const glm::vec3& origin, const glm::vec3& direction, void*& data) const
{
	m_collisionWorld->computeOverlappingPairs();

	const auto end = origin + direction * 1000.0f;
	btVector3 rayFrom{ origin.x, origin.y, origin.z };
	btVector3 rayTo{ end.x, end.y, end.z };

	btCollisionWorld::ClosestRayResultCallback hitCallback{
		rayFrom, rayTo };

	m_collisionWorld->rayTest(rayFrom, rayTo, hitCallback);

	if (!hitCallback.hasHit()) {
		data = nullptr;
		return false;
	}

	data = hitCallback.m_collisionObject->getUserPointer();
	return true;
}

void app::CollisionWorld::add(const glm::vec3& center, float radius, void* data)
{
	auto object = std::make_unique<btCollisionObject>();
	object->getWorldTransform().setOrigin(btVector3{center.x, center. y, center.z});

	auto shape = std::make_unique<btSphereShape>(radius);

	object->setCollisionShape(shape.get());
	object->setUserPointer(data);

	m_collisionWorld->addCollisionObject(object.get());

	m_collisionObjects.emplace_back(CollisionObject{ std::move(object), std::move(shape) });
}

void app::CollisionWorld::clear()
{
	for (const auto& collider : m_collisionObjects) {
		m_collisionWorld->removeCollisionObject(collider.object.get());
	}

	m_collisionObjects.clear();
}
