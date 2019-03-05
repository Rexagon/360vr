#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ej 
{
	class Transform final
	{
	public:
		explicit Transform(Transform* parent = nullptr);

		void setParent(Transform* parent);
		Transform* getParent() const;

		glm::mat4 getGlobalTransformationMatrix() const;

		void setTransformationMatrix(const glm::mat4& transformation);
		glm::mat4 getTransformationMatrix() const;

		glm::mat4 getPositionMatrix() const;
		glm::mat4 getPositionMatrixInverse() const;

		glm::mat4 getRotationMatrix() const;
		glm::mat4 getRotationMatrixInverse() const;
		glm::mat4 getGlobalRotationMatrix() const;
		glm::mat4 getGlobalRotationMatrixInverse() const;

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

		Transform* m_parent{ nullptr };

		glm::vec3 m_position{0.0f};
		glm::quat m_rotation{1.0f, 0.0f, 0.0f, 0.0f};
		glm::vec3 m_scale{1.0f};

		mutable glm::vec3 m_directionFront{0.0f, 0.0f, -1.0f};
		mutable glm::vec3 m_directionRight{1.0f, 0.0f, 0.0f};
		mutable glm::vec3 m_directionUp{0.0f, 0.0f, 0.0f};

		mutable glm::mat4 m_transformation{1.0f};

		mutable glm::mat4 m_positionMatrix{1.0f};
		mutable glm::mat4 m_positionMatrixInverse{1.0f};

		mutable glm::mat4 m_rotationMatrix{1.0f};
		mutable glm::mat4 m_rotationMatrixInverse{1.0f};

		mutable glm::mat4 m_scaleMatrix{1.0f};
		mutable glm::mat4 m_scaleMatrixInverse{1.0f};

		mutable bool m_positionChanged{ true };
		mutable bool m_rotationChanged{ true };
		mutable bool m_scaleChanged{ true };

		mutable bool m_transformationChanged{ true };
	};
}
