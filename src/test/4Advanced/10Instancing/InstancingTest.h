#pragma once
#include "Test.h"
#include<unordered_map>
class Shader;
class Model;


class InstancingTest :
    public Test
{
	//тсоН
	glm::vec2 translations[100];
	vector<glm::mat4> modelMatrices;
	int amount = 10000;


	unsigned int texHDR;
	unordered_map<string, shared_ptr<Shader>> shaders;
	string CurrentShader = "Instancing";

	shared_ptr<Model> planet;
	shared_ptr<Model> rock;
	shared_ptr<Model> sphere;
public:
	InstancingTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

