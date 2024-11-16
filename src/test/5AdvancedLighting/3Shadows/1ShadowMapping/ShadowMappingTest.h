#pragma once
#include "Test.h"
#include<unordered_map>
class Shader;
class ShadowMappingTest :
    public Test
{
	const int SHADOW_WIDTH=2048;
	const int SHADOW_HEIGHT= 2048;

	unsigned int DepthFBO;
	unsigned int DepthTexture;
	bool DrawDepth = false;
	unordered_map<string, shared_ptr<Shader>>Shaders;
public:
	ShadowMappingTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
private:
	void RenderScene(shared_ptr<Shader>shader);
};

