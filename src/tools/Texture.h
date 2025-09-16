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
    static void UnBind();
    static void Bind(unsigned int slot, unsigned int id);
    static void UnBind( unsigned int id);

	static void SetRound(unsigned int mode);
	static void SetFilter(unsigned int mode);
};

