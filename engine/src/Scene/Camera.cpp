#include "Scene/Camera.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace ej;

Camera::Camera() :
	m_depthRange(0.1f, 100.0f), m_projectionType(PERSPECTIVE), m_fov(glm::half_pi<float>() * 0.77f),
	m_aspect(1.0f), m_left(0.0f), m_right(0.0f), m_bottom(0.0f), m_top(0.0f), m_projectionChanged(true)
{
}

Camera::Camera(float minDepth, float maxDepth) :
	m_depthRange(minDepth, maxDepth), m_projectionType(PERSPECTIVE), m_fov(glm::half_pi<float>() * 0.77f),
	m_aspect(1.0f), m_left(0.0f), m_right(0.0f), m_bottom(0.0f), m_top(0.0f), m_projectionChanged(true)
{
}

Camera::Camera(const glm::vec2& zRange) :
	m_depthRange(zRange), m_projectionType(PERSPECTIVE), m_fov(glm::half_pi<float>() * 0.77f),
	m_aspect(1.0f), m_left(0.0f), m_right(0.0f), m_bottom(0.0f), m_top(0.0f), m_projectionChanged(true)
{
}

Camera::Camera(const glm::mat4& projection) :
	m_depthRange(0.0f, 0.0f),
	m_projectionType(CUSTOM), m_fov(0.0f), m_aspect(1.0f),
	m_left(0.0f), m_right(0.0f), m_bottom(0.0f), m_top(0.0f), m_projectionMatrix(projection), m_projectionChanged(false)
{
}

glm::mat4 Camera::getViewProjectionMatrix() const
{
	if (m_viewProjectionChanged) {
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
		m_viewProjectionChanged = false;
	}

	return m_viewProjectionMatrix;
}

glm::mat4 Camera::getViewMatrix() const
{
	return m_viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return m_projectionMatrix;
}

void Camera::setMinDepth(float minDepth)
{
	m_depthRange.x = minDepth;
	m_projectionChanged = true;
}

float Camera::getMinDepth() const
{
	return m_depthRange.x;
}

void Camera::setMaxDepth(float maxDepth)
{
	m_depthRange.y = maxDepth;
	m_projectionChanged = true;
}

float Camera::getMaxDepth() const
{
	return m_depthRange.y;
}

void Camera::setDepthRange(const glm::vec2 & depthRange)
{
	m_depthRange = depthRange;
	m_projectionChanged = true;
}

glm::vec2 Camera::getDepthRange() const
{
	return m_depthRange;
}

void Camera::updateView(const glm::mat4& globalTransformation)
{
	if (globalTransformation != m_globalTransformation) {
		m_viewMatrix = glm::inverse(globalTransformation);

		m_globalTransformation = globalTransformation;
		m_viewProjectionChanged = true;
	}
}

void Camera::updateProjection()
{
	if (m_projectionChanged) {
		switch (m_projectionType) {
		case PERSPECTIVE:
		{
			float f = 1.0f / tan(m_fov / 2.0f);

			/*m_projectionMatrix = glm::mat4(
				f / m_aspect, 0.0f, 0.0f, 0.0f,
				0.0f, f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, -1.0f,
				0.0f, 0.0f, m_depthRange.x, 0.0f);*/

			m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_depthRange.x, m_depthRange.y);

			break;
		}

		case ISOMETRIC:
		{
			m_projectionMatrix = glm::ortho(m_left, m_right,
				m_bottom, m_top, m_depthRange.x, m_depthRange.y);
			break;
		}

		case CUSTOM:
			break;
		}

		m_projectionChanged = false;
		m_viewProjectionChanged = true;
	}
}

void Camera::setProjectionType(ProjectionType type)
{
	if (type != m_projectionType) {
		m_projectionType = type;
		m_projectionChanged = true;
	}
}

Camera::ProjectionType Camera::getProjectionType() const
{
	return m_projectionType;
}

void Camera::setFieldOfView(float fov)
{
	if (fov != m_fov) {
		m_fov = fov;

		if (m_projectionType == PERSPECTIVE) {
			m_projectionChanged = true;
		}
	}
}

float Camera::getFieldOfView() const
{
	return m_fov;
}

void Camera::setAspect(float aspect)
{
	if (aspect != m_aspect) {
		m_aspect = aspect;

		if (m_projectionType == PERSPECTIVE) {
			m_projectionChanged = true;
		}
	}
}

float Camera::getAspect() const
{
	return m_aspect;
}

void Camera::setDimensions(float leftAndRight, float bottomAndTop)
{
	if (m_right != -m_left || m_right != leftAndRight || m_top != -m_bottom || m_top != bottomAndTop) {
		m_right = leftAndRight;
		m_left = -leftAndRight;
		m_top = bottomAndTop;
		m_bottom = -bottomAndTop;

		if (m_projectionType == ISOMETRIC) {
			m_projectionChanged = true;
		}
	}
}

void Camera::setDimensions(float left, float right, float bottom, float top)
{
	if (m_left != left || m_right != right || m_bottom != bottom || m_top != top) {
		m_left = left;
		m_right = right;
		m_bottom = bottom;
		m_top = top;

		if (m_projectionType == ISOMETRIC) {
			m_projectionChanged = true;
		}
	}
}

float Camera::getLeftDimension() const
{
	return m_left;
}

float Camera::getRightDimension() const
{
	return m_right;
}

float Camera::getBottomDimension() const
{
	return m_bottom;
}

float Camera::getTopDimension() const
{
	return m_top;
}