#pragma once

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "BaseManager.h"
#include "Shader.h"
#include "FrameBuffer.h"

namespace ej
{
	class WindowManager;

	class RenderingManager : public BaseManager, public PointerDefs<RenderingManager>
	{
	public:
		explicit RenderingManager(const Core& core);

		void apply();

		void setViewport(const glm::ivec2& size, const glm::ivec2& offset = glm::ivec2());
		void setViewport(GLint x, GLint y, GLsizei width, GLsizei height);
		glm::ivec2 getViewportSize() const;
		glm::ivec2 getViewportOffset() const;

		void setClearColor(const glm::vec4& color);
		void setClearColor(float r, float g, float b, float a = 1.0f);
		glm::vec4 getClearColor() const;

		void setClearDepth(float depth);
		float getClearDepth() const;

		void setDepthTestEnabled(bool enabled);
		bool isDepthTestEnabled() const;

		void setDepthWriteEnabled(bool enabled);
		bool isDepthWriteEnabled() const;

		void setDepthTestFunction(GLenum testFunction);
		GLenum getDepthTestFunction() const;

		void setClipControl(GLenum origin, GLenum depth);
		GLenum getClipControlOrigin() const;
		GLenum getClipControlDepth() const;

		void setBlendingEnabled(bool enabled);
		bool isBlendingEnabled() const;

		void setBlendingFunction(GLenum src, GLenum dst);
		GLenum getBlendingFunctionSrc() const;
		GLenum getBlendingFunctionDst() const;

		void setFaceCullingEnabled(bool enabled);
		bool isFaceCullingEnabled() const;

		void setFaceCullingSide(GLenum side);
		GLenum getFaceCullingSide() const;

		void setPolygonMode(GLenum mode);
		GLenum getPolygonMode() const;

		void setCurrentShader(const Shader* shader);
		const Shader* getCurrentShader() const;

		void setCurrentFrameBuffer(const FrameBuffer* frameBuffer);
		const FrameBuffer* getCurrentFrameBuffer() const;

	private:
		void setEnabled(GLenum prop, bool enabled);

		std::shared_ptr<WindowManager> m_windowManager;

		GLint m_viewport[4];

		GLclampf m_clearColor[4];

		float m_clearDepth;

		bool m_isDepthTestEnabled;
		bool m_isDepthWriteEnabled;
		GLenum m_depthTestFunction;

		GLenum m_clipControlOrigin;
		GLenum m_clipControlDepth;

		bool m_isBlendingEnabled;
		GLenum m_blendingFunctionSrc;
		GLenum m_blendingFunctionDst;

		bool m_isFaceCullingEnabled;
		GLenum m_faceCullingSide;

		GLenum m_polygonMode;

		const Shader* m_currentShader;
		const FrameBuffer* m_currentFrameBuffer;
	};
}
