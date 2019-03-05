#include "Scene/Camera.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

ej::Camera::Camera() :
	m_depthRange(0.1f, 100.0f), m_projectionType(Perspective), 
	m_fov(glm::half_pi<float>() * 0.77f), m_aspect(1.0f)
{
}

ej::Camera::Camera(const float minDepth, const float maxDepth) :
	m_depthRange(minDepth, maxDepth), m_projectionType(Perspective), 
	m_fov(glm::half_pi<float>() * 0.77f), m_aspect(1.0f)
{
}

ej::Camera::Camera(const glm::vec2& zRange) :
	m_depthRange(zRange), m_projectionType(Perspective), 
	m_fov(glm::half_pi<float>() * 0.77f), m_aspect(1.0f)
{
}

ej::Camera::Camera(const glm::mat4& projection) :
	m_depthRange(0.0f, 0.0f), m_projectionMatrix(projection)
{
}

glm::mat4 ej::Camera::getViewProjectionMatrix() const
{
	if (m_viewProjectionChanged) {
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
		m_viewProjectionChanged = false;
	}

	return m_viewProjectionMatrix;
}

glm::mat4 ej::Camera::getViewMatrix() const
{
	return m_viewMatrix;
}

glm::mat4 ej::Camera::getProjectionMatrix() const
{
	return m_projectionMatrix;
}

void ej::Camera::setMinDepth(const float minDepth)
{
	m_depthRange.x = minDepth;
	m_projectionChanged = true;
}

float ej::Camera::getMinDepth() const
{
	return m_depthRange.x;
}

void ej::Camera::setMaxDepth(const float maxDepth)
{
	m_depthRange.y = maxDepth;
	m_projectionChanged = true;
}

float ej::Camera::getMaxDepth() const
{
	return m_depthRange.y;
}

void ej::Camera::setDepthRange(const glm::vec2 & depthRange)
{
	m_depthRange = depthRange;
	m_projectionChanged = true;
}

glm::vec2 ej::Camera::getDepthRange() const
{
	return m_depthRange;
}

void ej::Camera::updateView(const glm::mat4& globalTransformation)
{
	if (globalTransformation != m_globalTransformation) {
		m_viewMatrix = glm::inverse(globalTransformation);

		m_globalTransformation = globalTransformation;
		m_viewProjectionChanged = true;
	}
}

void ej::Camera::updateProjection()
{
	if (m_projectionChanged) {
		switch (m_projectionType) {
		case Perspective:
		{
			/*
			float f = 1.0f / tan(m_fov / 2.0f);

			m_projectionMatrix = glm::mat4(
				f / m_aspect, 0.0f, 0.0f, 0.0f,
				0.0f, f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, -1.0f,
				0.0f, 0.0f, m_depthRange.x, 0.0f);
			*/

			m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_depthRange.x, m_depthRange.y);
			break;
		}

		case Isometric:
			m_projectionMatrix = glm::ortho(m_left, m_right,
				m_bottom, m_top, m_depthRange.x, m_depthRange.y);
			break;

		case Custom:
			break;
		}

		m_projectionChanged = false;
		m_viewProjectionChanged = true;
	}
}

void ej::Camera::setProjectionType(const ProjectionType type)
{
	if (type != m_projectionType) {
		m_projectionType = type;
		m_projectionChanged = true;
	}
}

ej::Camera::ProjectionType ej::Camera::getProjectionType() const
{
	return m_projectionType;
}

void ej::Camera::setFieldOfView(const float fov)
{
	if (fov != m_fov && m_projectionType == Perspective) {
		m_fov = fov;
		m_projectionChanged = true;
	}
}

float ej::Camera::getFieldOfView() const
{
	return m_fov;
}

void ej::Camera::setAspect(const float aspect)
{
	if (aspect != m_aspect && m_projectionType == Perspective) {
		m_aspect = aspect;
		m_projectionChanged = true;
	}
}

float ej::Camera::getAspect() const
{
	return m_aspect;
}

void ej::Camera::setDimensions(const float leftAndRight, const float bottomAndTop)
{
	if (m_projectionType != Isometric) {
		return;
	}

	if (m_right != -m_left || 
		m_right != leftAndRight || 
		m_top != -m_bottom || 
		m_top != bottomAndTop) 
	{
		m_right = leftAndRight;
		m_left = -leftAndRight;
		m_top = -bottomAndTop;
		m_bottom = bottomAndTop;
		m_projectionChanged = true;
	}
}

void ej::Camera::setDimensions(float left, float right, float bottom, float top)
{
	if (m_projectionType != Isometric) {
		return;
	}

	if (
		m_left != left || 
		m_right != right || 
		m_bottom != bottom || 
		m_top != top) 
	{
		m_left = left;
		m_right = right;
		m_bottom = bottom;
		m_top = top;
		m_projectionChanged = true;
	}
}

float ej::Camera::getLeftDimension() const
{
	return m_left;
}

float ej::Camera::getRightDimension() const
{
	return m_right;
}

float ej::Camera::getBottomDimension() const
{
	return m_bottom;
}

float ej::Camera::getTopDimension() const
{
	return m_top;
}

void ej::Camera::setCustomProjection(const glm::mat4& projection)
{
	m_projectionType = Custom;
	m_projectionMatrix = projection;
	m_projectionChanged = true;
}
