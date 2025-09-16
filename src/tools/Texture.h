#pragma once
#include<string>

class Texture
{

	int Width, Height;
	
public:
	std::string type;//diffuse specular
	std::string path;//用于比较·是否已加载过
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

