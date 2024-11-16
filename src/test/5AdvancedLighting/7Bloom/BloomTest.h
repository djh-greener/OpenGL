#pragma once
#include "Test.h"
class Shader;
class BloomTest :
    public Test
{


	vector<glm::vec3> LightPositions;
	vector<glm::vec3> LightColors;

	unordered_map<string, shared_ptr<Shader>>Shaders;

	unsigned int HDRTextures[2];
	unsigned int FBO;
	unsigned int pingpongFBO[2];
	unsigned int pingpongBuffer[2];

	string Texture2Show = "Default";
public:
	BloomTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
private:
	void RenderCube();
};

