#pragma once
#include "Test.h"
class Shader;
class Model;
class DeferredShadingTest :
    public Test
{
	unordered_map<string, shared_ptr<Shader>>Shaders;
	shared_ptr<Model >nanosuit;

	unsigned int GBuffer,GPosition,GNormal,GColorSpec;
	vector<glm::vec3>ObjectPositions;
	vector<glm::vec3> lightPositions;
	vector<glm::vec3> lightColors;

	bool isNormal = false;
public:
	DeferredShadingTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

