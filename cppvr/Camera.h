#pragma once

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include "PointerDefs.h"

namespace ej {
	class Camera : public PointerDefs<Camera>
	{
	public:
		enum ProjectionType
		{
			PERSPECTIVE,
			ISOMETRIC,
			CUSTOM
		};

		Camera();
		Camera(float minDepth, float maxDepth);
		Camera(const glm::vec2& zRange);
		Camera(const glm::mat4& projection);

		void updateView(const glm::mat4& globalTransformation);
		void updateProjection();

		void setProjectionType(ProjectionType type);
		ProjectionType getProjectionType() const;

		// perspective projection functions:
		void setFieldOfView(float fov);
		float getFieldOfView() const;

		void setAspect(float aspect);
		float getAspect() const;
		//

		// isometric projection functions:
		void setDimensions(float leftAndRight, float bottomAndTop);
		void setDimensions(float left, float right, float bottom, float top);

		float getLeftDimension() const;
		float getRightDimension() const;
		float getBottomDimension() const;
		float getTopDimension() const;
		//

		glm::mat4 getViewProjectionMatrix() const;

		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix() const;

		void setMinDepth(float minDepth);
		float getMinDepth() const;

		void setMaxDepth(float maxDepth);
		float getMaxDepth() const;

		void setDepthRange(const glm::vec2& depthRange);
		glm::vec2 getDepthRange() const;

	protected:
		glm::vec2 m_depthRange;

		ProjectionType m_projectionType;

		float m_fov;
		float m_aspect;

		float m_left;
		float m_right;
		float m_bottom;
		float m_top;

		glm::mat4 m_viewMatrix;
		glm::mat4 m_globalTransformation;

		glm::mat4 m_projectionMatrix;
		bool m_projectionChanged;

		mutable glm::mat4 m_viewProjectionMatrix;
		mutable bool m_viewProjectionChanged;

	};
}
