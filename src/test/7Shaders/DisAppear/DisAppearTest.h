#pragma once
#include "Test.h"

class Model;
class Shader;
class Vertex;
class DisAppearTest :
    public Test
{
	shared_ptr<Model>model;
	unordered_map<string, shared_ptr<Shader>>Shaders;
public:
	DisAppearTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

