#pragma once
#include "Test.h"
class Shader;
class Model;
class SSAOTest :
    public Test
{
	unordered_map<string, shared_ptr<Shader>>Shaders;
	shared_ptr<Model >nanosuit;

	unsigned int GBuffer, GPosition, GNormal, GColorSpec;
	unsigned int noiseTexture;
	unsigned int SSAOFBO, SSAOColorBuffer;
	glm::vec3 nanosuit_pos = { 0, 1.0, 8 };
	vector<glm::vec3> ssaoKernel;
	vector<glm::vec3> ssaoNoise;
	int DrawMethod = 0;
public:
	SSAOTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

