#pragma once
#include "Test.h"
#include <memory>
using namespace std;

class BlendTest :
    public Test
{
	glm::vec3 lightColor = { 1,1,1 };
	glm::vec3 lightDir = { 1,1,1 };
	string transparent = "Grass";
public:
	BlendTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

