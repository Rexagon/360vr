#pragma once

#include <string>

#include "Transform.h"

namespace ej
{
	class Entity
	{
	public:
		virtual ~Entity() = default;

		void setName(const std::string& name);
		const std::string& getName() const;

		void setTransform(const Transform& transform);
		Transform& getTransform();
		const Transform& getTransform() const;

	protected:
		Entity() = default;

		std::string m_name;

		Transform m_transform;
	};
}