#include "FrameBuffer.h"

#include"Texture.h"

#include"MyDebug.h"

static GLenum AttachmentTypeToGL(AttachmentType& format)
{
    switch (format)
    {
        case AttachmentType::RGBA8:							return GL_RGBA8;
        case AttachmentType::RGBA16F:						return GL_RGBA16F;
        case AttachmentType::RED_INTEGER:					return GL_RED_INTEGER;
        case AttachmentType::RED_FLOAT16:                   return GL_R16F;
        case AttachmentType::DEPTH24STENCIL8:				return GL_DEPTH24_STENCIL8;
    }

    return 0;
}

static void AttachColorTexture(uint32_t id, FramebufferSpecification& spec, GLenum internalFormat, GLenum format, int index)
{
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, spec.Width, spec.Height, 0, format, GL_UNSIGNED_BYTE,nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, id, 0);
}

static void AttachDepthTexture(uint32_t id, FramebufferSpecification& spec, GLenum format, GLenum attachmentType)
{
    glTexStorage2D(GL_TEXTURE_2D, 1, format, spec.Width, spec.Height);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, id, 0);
}

static bool IsDepthFormat(AttachmentType& format)
{
    switch (format)
    {
        case AttachmentType::DEPTH24STENCIL8:	return true;
    }
    return false;
}

FrameBuffer::FrameBuffer(const FramebufferSpecification& spec):m_Specification(spec)
{
    for (auto spec : m_Specification.Attachments.Attachments)
    {
        if (!IsDepthFormat(spec))
            m_ColorAttachmentSpecifications.emplace_back(spec);
        else
            m_DepthAttachmentSpecification = spec;
    }
    Invalidate();
}

FrameBuffer::~FrameBuffer()
{

    glDeleteFramebuffers(1, &m_RendererID);
    glDeleteTextures((int)m_ColorAttachments.size(), m_ColorAttachments.data());
    glDeleteTextures(1, &m_DepthAttachment);

}

void FrameBuffer::Invalidate() {
    if (m_RendererID)
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures((int)m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
        m_ColorAttachments.clear();
        m_DepthAttachment = 0;
    }
    glCreateFramebuffers(1, &m_RendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    //Add Color
    if (!m_ColorAttachmentSpecifications.empty())
    {
        m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
        glCreateTextures(GL_TEXTURE_2D, (int)m_ColorAttachments.size(), m_ColorAttachments.data());
        for (int i = 0; i < m_ColorAttachments.size(); ++i)
        {
            AttachmentType TextureFormat = m_ColorAttachmentSpecifications[i];
            glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[i]);

            switch (TextureFormat)
            {
                case AttachmentType::RGBA8:
                    AttachColorTexture(m_ColorAttachments[i], m_Specification, GL_RGBA8,GL_RGBA, i);
                    break;
                case AttachmentType::RGBA16F:
                    AttachColorTexture(m_ColorAttachments[i], m_Specification, GL_RGBA16F, GL_RGBA, i);
                    break;
                case AttachmentType::RED_INTEGER:
                    AttachColorTexture(m_ColorAttachments[i], m_Specification, GL_R32I, GL_RED_INTEGER, i);
                    break;
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    //Add Depth
    if (m_DepthAttachmentSpecification!=AttachmentType::None)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        switch (m_DepthAttachmentSpecification)
        {
            case AttachmentType::DEPTH24STENCIL8:
                AttachDepthTexture(m_DepthAttachment, m_Specification, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);
                break;
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        cout<<"Framebuffer is incomplete!"<<endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind() const{
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glViewport(0, 0, (uint32_t)m_Specification.Width, (uint32_t)m_Specification.Height);
}

void FrameBuffer::UnBind() const{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
{
    if(attachmentIndex >= m_ColorAttachments.size())
        return;

    auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
    glClearTexImage(m_ColorAttachments[attachmentIndex], 0, AttachmentTypeToGL(spec), GL_INT, &value);
}

