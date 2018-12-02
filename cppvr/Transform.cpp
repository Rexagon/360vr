#include "Transform.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glm/gtx/matrix_decompose.hpp>

using namespace ej;

Transform::Transform() :
	m_position(0.0f, 0.0f, 0.0f), m_rotation(1.0f, 0.0f, 0.0f, 0.0f), m_scale(1.0f, 1.0f, 1.0f),
	m_directionFront(0.0f, 0.0f, -1.0f), m_directionRight(1.0f, 0.0f, 0.0f), m_directionUp(0.0f, 1.0f, 0.0f),
	m_transformation(1.0f),
	m_positionMatrix(1.0f), m_positionMatrixInversed(1.0f),
	m_rotationMatrix(1.0f), m_rotationMatrixInversed(1.0f),
	m_scaleMatrix(1.0f), m_scaleMatrixInversed(1.0f),
	m_positionChanged(true), m_rotationChanged(true), m_scaleChanged(true), m_transformationChanged(true)
{}

void Transform::setTransformationMatrix(const glm::mat4 & transform)
{
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(transform, m_scale, m_rotation, m_position, skew, perspective);
}

glm::mat4 Transform::getTransformationMatrix() const
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

glm::mat4 Transform::getPositionMatrix() const
{
	updatePosition();

	return m_positionMatrix;
}

glm::mat4 Transform::getPositionMatrixInversed() const
{
	updatePosition();

	return m_positionMatrixInversed;
}

glm::mat4 Transform::getRotationMatrix() const
{
	updateRotation();

	return m_rotationMatrix;
}

glm::mat4 Transform::getRotationMatrixInversed() const
{
	updateRotation();

	return m_rotationMatrixInversed;
}

glm::mat4 Transform::getScaleMatrix() const
{
	updateScale();

	return m_scaleMatrix;
}

glm::mat4 Transform::getScaleMatrixInversed() const
{
	updateScale();

	return m_scaleMatrixInversed;
}

void Transform::move(float x, float y, float z)
{
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
	m_positionChanged = true;
}

void Transform::move(const glm::vec3 & vector)
{
	m_position += vector;
	m_positionChanged = true;
}

void Transform::setPosition(float x, float y, float z)
{
	m_position = glm::vec3(x, y, z);
	m_positionChanged = true;
}

void Transform::setPosition(const glm::vec3 & position)
{
	m_position = position;
	m_positionChanged = true;
}

glm::vec3 Transform::getPosition() const
{
	return m_position;
}

void Transform::rotate(float x, float y, float z)
{
	m_rotation = glm::quat(glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z))) * m_rotation;
	m_rotation = glm::normalize(m_rotation);
	m_rotationChanged = true;
}

void Transform::rotate(const glm::vec3 & eulerAngles)
{
	m_rotation = glm::quat(glm::radians(eulerAngles)) * m_rotation;
	m_rotation = glm::normalize(m_rotation);
	m_rotationChanged = true;
}

void Transform::rotate(const glm::quat & rotation)
{
	m_rotation = rotation * m_rotation;
	m_rotation = glm::normalize(m_rotation);
	m_rotationChanged = true;
}

void Transform::setRotation(float x, float y, float z)
{
	m_rotation = glm::quat(glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z)));
	m_rotation = glm::normalize(m_rotation);
	m_rotationChanged = true;
}

void Transform::setRotation(const glm::vec3 & eulerAngles)
{
	m_rotation = glm::quat(glm::radians(eulerAngles));
	m_rotation = glm::normalize(m_rotation);
	m_rotationChanged = true;
}

void Transform::setRotation(const glm::quat & rotation)
{
	m_rotation = rotation;
	m_rotation = glm::normalize(m_rotation);
	m_rotationChanged = true;
}

glm::quat Transform::getRotation() const
{
	return m_rotation;
}

glm::vec3 Transform::getEulerRotation() const
{
	return glm::eulerAngles(m_rotation);
}

void Transform::scale(float s)
{
	m_scale *= s;
	m_scaleChanged = true;
}

void Transform::scale(float x, float y, float z)
{
	m_scale *= glm::vec3(x, y, z);
	m_scaleChanged = true;
}

void Transform::scale(const glm::vec3 & s)
{
	m_scale *= s;
	m_scaleChanged = true;
}

void Transform::setScale(float s)
{
	m_scale = glm::vec3(s, s, s);
	m_scaleChanged = true;
}

void Transform::setScale(float x, float y, float z)
{
	m_scale = glm::vec3(x, y, z);
	m_scaleChanged = true;
}

void Transform::setScale(const glm::vec3 & s)
{
	m_scale = s;
	m_scaleChanged = true;
}

glm::vec3 Transform::getScale() const
{
	return m_scale;
}

glm::vec3 Transform::getDirectionFront() const
{
	updateRotation();

	return m_directionFront;
}

glm::vec3 Transform::getDirectionRight() const
{
	updateRotation();

	return m_directionRight;
}

glm::vec3 Transform::getDirectionUp() const
{
	updateRotation();

	return m_directionUp;
}

void Transform::updatePosition() const
{
	if (m_positionChanged) {
		m_positionMatrix = glm::translate(glm::mat4(1.0f), m_position);
		m_positionMatrixInversed = glm::translate(glm::mat4(1.0f), -m_position);

		m_positionChanged = false;
		m_transformationChanged = true;
	}
}

void Transform::updateRotation() const
{
	if (m_rotationChanged) {
		m_rotationMatrix = glm::mat4_cast(m_rotation);
		m_rotationMatrixInversed = glm::inverse(m_rotationMatrix);

		// update direction also
		glm::vec4 temp(0.0f, 0.0f, -1.0f, 1.0f);
		temp = m_rotationMatrix * temp;
		temp /= temp.w;
		glm::vec3 directionFront(temp.x, temp.y, temp.z);

		m_directionFront = glm::normalize(directionFront);
		m_directionRight = glm::cross(m_directionFront, glm::vec3(0.0f, 1.0f, 0.0f));
		m_directionUp = glm::cross(m_directionRight, m_directionFront);

		m_rotationChanged = false;
		m_transformationChanged = true;
	}
}

void Transform::updateScale() const
{
	if (m_scaleChanged) {
		m_scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
		m_scaleMatrixInversed = glm::scale(glm::mat4(1.0f),
		                                   glm::vec3(1.0f / m_scale.x, 1.0f / m_scale.y, 1.0f / m_scale.z));

		m_scaleChanged = false;
		m_transformationChanged = true;
	}
}