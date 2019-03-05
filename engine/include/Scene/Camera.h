#pragma once

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

namespace ej {
	class Camera final
	{
	public:
		enum ProjectionType
		{
			Perspective,
			Isometric,
			Custom
		};

		Camera();
		Camera(float minDepth, float maxDepth);
		explicit Camera(const glm::vec2& zRange);
		explicit Camera(const glm::mat4& projection);

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

		void setCustomProjection(const glm::mat4& projection);

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
		glm::vec2 m_depthRange{0.0f};

		ProjectionType m_projectionType{ Custom };

		float m_fov{ 0.0f };
		float m_aspect{ 0.0f };

		float m_left{0.0f};
		float m_right{0.0f};
		float m_bottom{0.0f};
		float m_top{0.0f};

		glm::mat4 m_viewMatrix{1.0f};
		glm::mat4 m_globalTransformation{1.0f};

		glm::mat4 m_projectionMatrix{1.0f};
		bool m_projectionChanged{true};

		mutable glm::mat4 m_viewProjectionMatrix{1.0f};
		mutable bool m_viewProjectionChanged{true};
	};
}
