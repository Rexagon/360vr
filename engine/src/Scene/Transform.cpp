#include "Scene/Transform.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glm/gtx/matrix_decompose.hpp>

ej::Transform::Transform(Transform* parent) :
	m_parent(parent)
{}

void ej::Transform::setParent(Transform* parent)
{
	m_parent = parent;
}

ej::Transform* ej::Transform::getParent() const
{
	return m_parent;
}

glm::mat4 ej::Transform::getGlobalTransformationMatrix() const
{
	if (m_parent == nullptr) {
		return getTransformationMatrix();
	}

	return m_parent->getGlobalTransformationMatrix() * getTransformationMatrix();
}

void ej::Transform::setTransformationMatrix(const glm::mat4 & transformation)
{
	glm::vec3 skew;
	glm::vec4 perspective;

	decompose(transformation, m_scale, m_rotation, m_position, skew, perspective);
	m_positionChanged = true;
	m_rotationChanged = true;
	m_scaleChanged = true;
}

glm::mat4 ej::Transform::getTransformationMatrix() const
{
	updatePosition();
	updateRotation();
	updateScale();

	if (m_transformationChanged) {
		m_transformation = getPositionMatrix() * getRotationMatrix() * getScaleMatrix();
		m_transformationChanged = false;
	}

	return m_transformation;
}

glm::mat4 ej::Transform::getPositionMatrix() const
{
	updatePosition();

	return m_positionMatrix;
}

glm::mat4 ej::Transform::getPositionMatrixInverse() const
{
	updatePosition();

	return m_positionMatrixInverse;
}

glm::mat4 ej::Transform::getRotationMatrix() const
{
	updateRotation();

	return m_rotationMatrix;
}

glm::mat4 ej::Transform::getRotationMatrixInverse() const
{
	updateRotation();

	return m_rotationMatrixInverse;
}

glm::mat4 ej::Transform::getGlobalRotationMatrix() const
{
	if (m_parent == nullptr) {
		return getRotationMatrix();
	}

	return m_parent->getRotationMatrix() * getRotationMatrix();
}

glm::mat4 ej::Transform::getGlobalRotationMatrixInverse() const
{
	if (m_parent == nullptr) {
		return getRotationMatrixInverse();
	}

	return getRotationMatrixInverse() * m_parent->getRotationMatrixInverse();
}

glm::mat4 ej::Transform::getScaleMatrix() const
{
	updateScale();

	return m_scaleMatrix;
}

glm::mat4 ej::Transform::getScaleMatrixInverse() const
{
	updateScale();

	return m_scaleMatrixInverse;
}

void ej::Transform::move(float x, float y, float z)
{
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
	m_positionChanged = true;
}

void ej::Transform::move(const glm::vec3 & vector)
{
	m_position += vector;
	m_positionChanged = true;
}

void ej::Transform::setPosition(float x, float y, float z)
{
	m_position = glm::vec3(x, y, z);
	m_positionChanged = true;
}

void ej::Transform::setPosition(const glm::vec3 & position)
{
	m_position = position;
	m_positionChanged = true;
}

glm::vec3 ej::Transform::getPosition() const
{
	return m_position;
}

glm::vec3 ej::Transform::getGlobalPosition() const
{
	if (m_parent == nullptr) {
		return getPosition();
	}

	return m_parent->getGlobalTransformationMatrix() * glm::vec4(getPosition(), 1.0f);
}

void ej::Transform::rotate(float x, float y, float z)
{
	m_rotation = glm::quat(glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z))) * m_rotation;
	m_rotation = normalize(m_rotation);
	m_rotationChanged = true;
}

void ej::Transform::rotate(const glm::vec3 & eulerAngles)
{
	m_rotation = glm::quat(radians(eulerAngles)) * m_rotation;
	m_rotation = normalize(m_rotation);
	m_rotationChanged = true;
}

void ej::Transform::rotate(const glm::quat & rotation)
{
	m_rotation = rotation * m_rotation;
	m_rotation = normalize(m_rotation);
	m_rotationChanged = true;
}

void ej::Transform::setRotation(float x, float y, float z)
{
	m_rotation = glm::quat(glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z)));
	m_rotation = normalize(m_rotation);
	m_rotationChanged = true;
}

void ej::Transform::setRotation(const glm::vec3 & eulerAngles)
{
	m_rotation = glm::quat(glm::radians(eulerAngles));
	m_rotation = normalize(m_rotation);
	m_rotationChanged = true;
}

void ej::Transform::setRotation(const glm::quat & rotation)
{
	m_rotation = rotation;
	m_rotation = normalize(m_rotation);
	m_rotationChanged = true;
}

glm::quat ej::Transform::getRotation() const
{
	return m_rotation;
}

glm::vec3 ej::Transform::getEulerRotation() const
{
	return eulerAngles(m_rotation);
}

void ej::Transform::scale(float s)
{
	m_scale *= s;
	m_scaleChanged = true;
}

void ej::Transform::scale(float x, float y, float z)
{
	m_scale *= glm::vec3(x, y, z);
	m_scaleChanged = true;
}

void ej::Transform::scale(const glm::vec3 & s)
{
	m_scale *= s;
	m_scaleChanged = true;
}

void ej::Transform::setScale(float s)
{
	m_scale = glm::vec3(s, s, s);
	m_scaleChanged = true;
}

void ej::Transform::setScale(float x, float y, float z)
{
	m_scale = glm::vec3(x, y, z);
	m_scaleChanged = true;
}

void ej::Transform::setScale(const glm::vec3 & s)
{
	m_scale = s;
	m_scaleChanged = true;
}

glm::vec3 ej::Transform::getScale() const
{
	return m_scale;
}

glm::vec3 ej::Transform::getDirectionFront() const
{
	updateRotation();

	return m_directionFront;
}

glm::vec3 ej::Transform::getDirectionRight() const
{
	updateRotation();

	return m_directionRight;
}

glm::vec3 ej::Transform::getDirectionUp() const
{
	updateRotation();

	return m_directionUp;
}

void ej::Transform::updatePosition() const
{
	if (m_positionChanged) {
		m_positionMatrix = translate(glm::mat4(1.0f), m_position);
		m_positionMatrixInverse = translate(glm::mat4(1.0f), -m_position);

		m_positionChanged = false;
		m_transformationChanged = true;
	}
}

void ej::Transform::updateRotation() const
{
	if (m_rotationChanged) {
		m_rotationMatrix = glm::mat4_cast(m_rotation);
		m_rotationMatrixInverse = glm::inverse(m_rotationMatrix);

		// update direction also
		glm::vec4 temp(0.0f, 0.0f, -1.0f, 1.0f);
		temp = m_rotationMatrix * temp;
		temp /= temp.w;
		const glm::vec3 directionFront(temp.x, temp.y, temp.z);

		m_directionFront = normalize(directionFront);
		m_directionRight = normalize(cross(m_directionFront, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_directionUp = normalize(cross(m_directionRight, m_directionFront));

		m_rotationChanged = false;
		m_transformationChanged = true;
	}
}

void ej::Transform::updateScale() const
{
	if (m_scaleChanged) {
		m_scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
		m_scaleMatrixInverse = glm::scale(glm::mat4(1.0f),
			glm::vec3(1.0f / m_scale.x, 1.0f / m_scale.y, 1.0f / m_scale.z));

		m_scaleChanged = false;
		m_transformationChanged = true;
	}
}