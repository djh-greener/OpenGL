#pragma once
#include "Test.h"
#include <memory>
using namespace std;
class Model;
class Shader;

class ModelLoadingTest :public Test
{
	shared_ptr<Model>model;
	shared_ptr<Shader>shader;

	glm::vec3 lightColor = glm::vec3(1);

	glm::vec3 dirLightdir;

	glm::vec3 pointLightPositions[4] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
	};
public:
    ModelLoadingTest();
    virtual void OnRender()override;
    virtual void OnImGuiRender()override;
};

