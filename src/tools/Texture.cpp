#include "Texture.h"
#include"stb_image/stb_image.h"
#include"glad/glad.h"
#include"MyDebug.h"

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;


Texture::Texture(std::string const ImgPath, bool gamma):path(ImgPath),Width(0),Height(0)
{
	//空纹理
	if (ImgPath.empty()) {
		GLCall(glGenTextures(1, &m_RenderID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		return;
	}
	stbi_set_flip_vertically_on_load(0);
	int nrComponents;
	GLenum internalFormat;
	GLenum dataFormat;
	unsigned char* data = stbi_load(ImgPath.c_str(), &Width, &Height, &nrComponents, 0);

	GLCall(glGenTextures(1, &m_RenderID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderID));
	if (nrComponents == 1)
	{
		internalFormat = dataFormat = GL_RED;
	}
	else if (nrComponents == 3)
	{
		internalFormat = gamma ? GL_SRGB : GL_RGB;
		dataFormat = GL_RGB;
	}
	else if (nrComponents == 4)
	{
		internalFormat = gamma ? GL_SRGB_ALPHA : GL_RGBA;
		dataFormat = GL_RGBA;
	}
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, Width, Height, 0, dataFormat, GL_UNSIGNED_BYTE, data));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	UnBind();
	if (data)
		stbi_image_free(data);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RenderID));
}

void Texture::Bind(unsigned int slot)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderID));
}

void Texture::UnBind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::SetRound(unsigned int mode)
{
	if (mode == 0)//mode为0，重置为默认
	{
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		return;
	}
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode));
}

void Texture::SetFilter(unsigned int mode)
{
	if (mode == 0) //mode为0，重置为默认
	{
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		return;
	}
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode));
}

void Texture::Bind(unsigned int slot, unsigned int id) {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, id));
}