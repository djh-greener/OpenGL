#include "FrameBuffer.h"

#include"Texture.h"

#include"MyDebug.h"
extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

FrameBuffer::FrameBuffer()
{
	GLCall(glGenFramebuffers(1, &RenderID));
}

void FrameBuffer::AddAttachment(AttachmentType type)
{
	Bind();
	switch (type)
	{
	case Default://默认颜色缓冲添加到纹理，深度和模板添加到渲染缓冲对象
		AddColorAttachmentTexture();
		AddDepthStencilAttachmentRender();
		break;
	case Color:
		AddColorAttachmentTexture();
		break;
	case Depth_Stencil:
		AddDepthStencilAttachmentRender();
		break;
	default:
		break;
	}
}

void FrameBuffer::Bind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, RenderID));
}

void FrameBuffer::UnBind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

bool FrameBuffer::CompleteCheck() const
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Log::GetInstance() << Log::Level::Warning << "FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		return false;
	}
		
	return true;
}

FrameBuffer::~FrameBuffer()
{
	GLCall(glDeleteFramebuffers(1, &RenderID));
}

void FrameBuffer::AddColorAttachmentTexture()
{
	ColorBuffer = make_shared<Texture>("");
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorBuffer->m_RenderID, 0));
}

void FrameBuffer::AddDepthStencilAttachmentRender()
{
	unsigned int rbo;
	GLCall(glGenRenderbuffers(1, &rbo));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_WIDTH));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));
}