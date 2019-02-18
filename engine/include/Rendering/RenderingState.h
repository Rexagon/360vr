#pragma once

#include "Core/PointerDefs.h"
#include "Resources/Shader.h"
#include "Rendering/FrameBuffer.h"
#include "Managers/WindowManager.h"

namespace ej
{
	class RenderingState : public PointerDefs<RenderingState>
	{
	public:
		explicit RenderingState(const Core& core);

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

		void setActiveTexture(size_t unit);
		void bindTexture(const Texture* texture, size_t unit);

		void setCurrentShader(Shader* shader);
		const Shader* getCurrentShader() const;

		void setCurrentFrameBuffer(FrameBuffer* frameBuffer);
		const FrameBuffer* getCurrentFrameBuffer() const;

	private:
		static void setEnabled(GLenum prop, bool enabled);

		GLint m_viewport[4]{};

		GLclampf m_clearColor[4]{ 0.0f, 0.0f, 0.0f, 1.0f };

		float m_clearDepth = 1.0f;

		bool m_isDepthTestEnabled = true;
		bool m_isDepthWriteEnabled = true;
		GLenum m_depthTestFunction = GL_LEQUAL;

		GLenum m_clipControlOrigin = GL_LOWER_LEFT;
		GLenum m_clipControlDepth = GL_NEGATIVE_ONE_TO_ONE;

		bool m_isBlendingEnabled = false;
		GLenum m_blendingFunctionSrc = GL_SRC_ALPHA;
		GLenum m_blendingFunctionDst = GL_ONE_MINUS_SRC_ALPHA;

		bool m_isFaceCullingEnabled = true;
		GLenum m_faceCullingSide = GL_BACK;

		GLenum m_polygonMode = GL_FILL;

		size_t m_activeTextureUnit = 0;

		Shader* m_currentShader = nullptr;
		FrameBuffer* m_currentFrameBuffer = nullptr;
	};
}
