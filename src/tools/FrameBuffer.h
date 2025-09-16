#pragma once
#include<memory>
#include<vector>
using namespace std;
enum AttachmentType {
    None=0,

    //Color
    RGBA8,
    RGBA16F,
    RED_INTEGER,
    RED_FLOAT16,
    //Depth/Stencil
    DEPTH24STENCIL8,

    //Defaults
    Depth = DEPTH24STENCIL8,
};
struct AttachmentsSpec
{
    AttachmentsSpec() = default;
    AttachmentsSpec(std::initializer_list<AttachmentType> attachments)
            : Attachments(attachments) {}

    vector<AttachmentType> Attachments;
};
struct FramebufferSpecification
{
    AttachmentsSpec Attachments;
    uint32_t Width=0, Height=0;
};
class Texture;
class FrameBuffer
{
public:

	FrameBuffer(const FramebufferSpecification& spec);
    ~FrameBuffer();
    void Invalidate();
	void Bind()const;
	void UnBind()const;
    void ClearAttachment(uint32_t attachmentIndex, int value);
    [[nodiscard]] uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const{
        return m_ColorAttachments[index];
    }
    [[nodiscard]] uint32_t GetDepthAttachmentRendererID() const{
        return m_DepthAttachment;
    }
    [[nodiscard]] const FramebufferSpecification& GetSpecification() const{ return m_Specification; }
private:
    uint32_t m_RendererID=0;

    FramebufferSpecification m_Specification;
    std::vector<AttachmentType>m_ColorAttachmentSpecifications;
    AttachmentType m_DepthAttachmentSpecification = AttachmentType::None;
    std::vector<uint32_t> m_ColorAttachments;
    uint32_t m_DepthAttachment = 0;
};

