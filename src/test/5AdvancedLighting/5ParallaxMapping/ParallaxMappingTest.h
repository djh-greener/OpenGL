#pragma once
#include "Test.h"
class Vertex;
class Shader;
class ParallaxMappingTest :
    public Test
{
	unordered_map<string, shared_ptr<Shader>>Shaders;
	void CalcTB(Vertex& A, Vertex& B, Vertex& C);
public:
	ParallaxMappingTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

