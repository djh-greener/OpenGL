#pragma once
#include<memory>
using namespace std;
enum AttachmentType {
	Default,
	Color,
	Depth_Stencil,
};
class Texture;
class FrameBuffer
{
	unsigned int RenderID;

public:
	shared_ptr<Texture>ColorBuffer;

	FrameBuffer();
	void AddAttachment(AttachmentType type);
	void Bind();
	void UnBind();
	bool CompleteCheck()const;
	~FrameBuffer();
private:
	void AddColorAttachmentTexture();

	void AddDepthStencilAttachmentRender();


};

