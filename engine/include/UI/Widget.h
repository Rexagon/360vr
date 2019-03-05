#pragma once

#include <SFML/Window/Event.hpp>

#include "Scene/MeshEntity.h"

namespace ej
{
	/**
	 * \brief User interface primitive. Base class for all UI elements.
	 */
	class Widget
	{
	public:
		/**
		 * \param core Main core object
		 */
		explicit Widget(const Core& core);

		/**
		 * \brief Construct named widget
		 * \param core Main core object
		 * \param name Widget name. Should be unique
		 */
		Widget(const Core& core, const std::string& name);

		/**
		 * \brief Default destructor
		 */
		virtual ~Widget() = default;

		virtual void update(float dt) = 0;

		virtual void handleEvent(const sf::Event& event) {};

		void setName(const std::string& name);
		const std::string& getName() const;

		void setSize(const glm::vec2& size);
		glm::vec2 getSize() const;

		void setOrigin(const glm::vec2& origin);
		glm::vec2 getOrigin() const;

		void setZIndex(float zIndex);
		float getZIndex() const;

		Transform& getTransform();

		MeshEntity* getMeshEntity();

	protected:
		void initMesh(Mesh* mesh, Material* material);

		std::string m_name;
		MeshEntity m_meshEntity{nullptr, nullptr};
		Transform m_transform;
	};
}
