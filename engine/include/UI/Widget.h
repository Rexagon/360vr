#pragma once

#include <SFML/Window/Event.hpp>

#include "Scene/MeshEntity.h"

namespace ej
{
	class Widget
	{
	public:
		Widget(const Core& core);
		Widget(const Core& core, const std::string& name);
		virtual ~Widget() = default;

		virtual void update(float dt) = 0;

		virtual void handleEvent(const sf::Event& event) {};

		void setName(std::string_view name);
		const std::string& getName() const;

		void setSize(const glm::vec2& size);
		glm::vec2 getSize() const;

		void setOrigin(const glm::vec2& origin);
		glm::vec2 getOrigin() const;

		void setZIndex(float zIndex);
		float getZIndex() const;

		Transform& getTransform();

		MeshEntity::ptr getMeshEntity() const;

	protected:
		void initMesh(Mesh::ptr mesh, std::shared_ptr<Material> material);

		std::string m_name;
		MeshEntity::ptr m_meshEntity;
		Transform m_transform;
	};
}
