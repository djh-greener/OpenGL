#pragma once
#include<string>

class Texture
{

	int Width, Height;
	
public:
	std::string type;//diffuse specular
	std::string path;//���ڱȽϡ��Ƿ��Ѽ��ع�
	unsigned int m_RenderID;
	Texture(std::string const ImgPath,bool gamma=false);
	~Texture();
	void Bind(unsigned int slot);
	void UnBind();

	static void SetRound(unsigned int mode);
	static void SetFilter(unsigned int mode);
};

