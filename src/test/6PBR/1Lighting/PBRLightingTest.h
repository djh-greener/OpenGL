#pragma once
#include "Test.h"
class Shader;
class IndexBuffer;

class PBRLightingTest :
    public Test
{
	unordered_map<string, shared_ptr<Shader>>Shaders;

	shared_ptr<IndexBuffer>ebo;

	float nrRows, nrCols;

	int nrLights;
	vector<glm::vec3> lightPositions, lightColors;

	bool RenderTextures = false;
public:
	PBRLightingTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

