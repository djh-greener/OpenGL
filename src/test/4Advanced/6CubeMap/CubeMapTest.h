#pragma once
#include "Test.h"
class Model;
class Shader;
class CubeMapTest :
    public Test
{
	int SkyBoxIdx = 8;//��պ������
	unsigned int textureID;//��պ�����
	shared_ptr<Model>model;
	shared_ptr<Shader>DefaultShader;

	shared_ptr<Shader>ReflectShader;
	shared_ptr<Shader>RefractShader;

public:
	CubeMapTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
	void LoadCubemap(vector<string> faces);
};

