#pragma once
#include "Test.h"

class Model;
class Shader;
class Vertex;
class NormalMappingTest :
    public Test
{
	shared_ptr<Model>model;
	unordered_map<string, shared_ptr<Shader>>Shaders;
	void CalcTB(Vertex& A, Vertex& B, Vertex& C);
public:
	NormalMappingTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

