#pragma once
#include "Test.h"
#include<unordered_map>
class Shader;
class PointShadowsTest:public Test
{
	unsigned int DepthCubeMap;
	unsigned int DepthFbo;
	const int SHADOW_WIDTH = 2048;
	const int SHADOW_HEIGHT = 2048;
	bool DrawDepth;
	unordered_map<string, shared_ptr<Shader>>Shaders;
public:
	PointShadowsTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
private:
	void RenderScene(shared_ptr<Shader>shader);

};

