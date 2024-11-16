#pragma once
#include "Test.h"
class Shader;
class AdvancedGLSLTest:public Test
{
	vector<glm::vec3> positions = {
	glm::vec3(-0.75f, 0.75f, 0.0f),
	glm::vec3(0.75f, 0.75f, 0.0f),
	glm::vec3(-0.75f, -0.75f, 0.0f),
	glm::vec3(0.75f, -0.75f, 0.0f)
	};
public:
	AdvancedGLSLTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

