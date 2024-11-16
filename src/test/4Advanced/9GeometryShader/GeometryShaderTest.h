#pragma once
#include "Test.h"
#include<unordered_map>
class Shader;
class Model;
class GeometryShaderTest:public Test
{
	unordered_map<string, shared_ptr<Shader>> shaders;
	string CurrentShaderName = "Point";
	shared_ptr<Model> model;

public:
	GeometryShaderTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

