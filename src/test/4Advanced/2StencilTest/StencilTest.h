#pragma once
#include "Test.h"
#include <memory>
using namespace std;

class Model;
class Shader;
class StencilTest :public Test
{
	shared_ptr<Model>model;
	shared_ptr<Shader>shader;
	shared_ptr<Shader>edge_shader;

	glm::vec3 lightColor = { 1,1,1 };
	glm::vec3 edgeColor = { 0,0,1 };
public:
	StencilTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

