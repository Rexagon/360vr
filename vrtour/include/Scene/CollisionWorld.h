#pragma once

#include <vector>
#include <functional>

#include <glm/vec3.hpp>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <BulletCollision/CollisionDispatch/btCollisionConfiguration.h>

namespace app
{
	class CollisionWorld final
	{
		struct CollisionObject final
		{
			std::unique_ptr<btCollisionObject> object;
			std::unique_ptr<btCollisionShape> shape;
		};

	public:
		using Callback = std::function<void(const glm::vec3&)>;

		CollisionWorld();

		bool raycast(const glm::vec3& origin, const glm::vec3& direction, void*& data) const;

		void add(const glm::vec3& center, float radius, void* data);

		void clear();

	private:
		std::unique_ptr<btCollisionConfiguration> m_collisionConfiguration;

		std::unique_ptr<btCollisionDispatcher> m_collisionDispatcher;
		std::unique_ptr<btBroadphaseInterface> m_broadphase;

		std::unique_ptr<btCollisionWorld> m_collisionWorld;

		std::vector<CollisionObject> m_collisionObjects;
	};
}
