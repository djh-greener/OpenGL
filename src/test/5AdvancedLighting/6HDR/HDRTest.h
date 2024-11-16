#pragma once
#include "Test.h"
class Shader;
class HDRTest :
    public Test
{
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
	unsigned int FBO, HDRTexture, RBODepth;
	unordered_map<string, shared_ptr<Shader>>Shaders;
	enum Algorithm { None , Reinhard, Exposure};
	Algorithm algorithm = Algorithm::None;
	float exposure = 1;
public:
	HDRTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

