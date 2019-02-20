#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Core/PointerDefs.h"

namespace ej 
{
	class Transform : public PointerDefs<Transform>
	{
	public:
		Transform(Transform* parent = nullptr);

		void setParent(Transform* parent);
		Transform* getParent() const;

		glm::mat4 getGlobalTransformationMatrix() const;

		void setTransformationMatrix(const glm::mat4& transformation);
		glm::mat4 getTransformationMatrix() const;

		glm::mat4 getPositionMatrix() const;
		glm::mat4 getPositionMatrixInverse() const;

		glm::mat4 getRotationMatrix() const;
		glm::mat4 getRotationMatrixInverse() const;

		glm::mat4 getScaleMatrix() const;
		glm::mat4 getScaleMatrixInverse() const;

		void move(float x, float y, float z);
		void move(const glm::vec3& vector);
		void setPosition(float x, float y, float z);
		void setPosition(const glm::vec3& position);
		glm::vec3 getPosition() const;
		glm::vec3 getGlobalPosition() const;

		void rotate(float x, float y, float z);
		void rotate(const glm::vec3& eulerAngles);
		void rotate(const glm::quat& rotation);
		void setRotation(float x, float y, float z);
		void setRotation(const glm::vec3& eulerAngles);
		void setRotation(const glm::quat& rotation);
		glm::quat getRotation() const;
		glm::vec3 getEulerRotation() const;

		void scale(float s);
		void scale(float x, float y, float z);
		void scale(const glm::vec3& s);
		void setScale(float s);
		void setScale(float x, float y, float z);
		void setScale(const glm::vec3& s);
		glm::vec3 getScale() const;

		glm::vec3 getDirectionFront() const;
		glm::vec3 getDirectionRight() const;
		glm::vec3 getDirectionUp() const;

	private:
		void updatePosition() const;
		void updateRotation() const;
		void updateScale() const;

		Transform* m_parent;

		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;

		mutable glm::vec3 m_directionFront;
		mutable glm::vec3 m_directionRight;
		mutable glm::vec3 m_directionUp;

		mutable glm::mat4 m_transformation;

		mutable glm::mat4 m_positionMatrix;
		mutable glm::mat4 m_positionMatrixInverse;

		mutable glm::mat4 m_rotationMatrix;
		mutable glm::mat4 m_rotationMatrixInverse;

		mutable glm::mat4 m_scaleMatrix;
		mutable glm::mat4 m_scaleMatrixInverse;

		mutable bool m_positionChanged;
		mutable bool m_rotationChanged;
		mutable bool m_scaleChanged;

		mutable bool m_transformationChanged;
	};
}
