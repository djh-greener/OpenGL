#pragma once
#include "Test.h"
struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

class MaterialTest :
    public Test
{
    string MatName = "emerald";
    glm::vec3 LightColor = { 1,1,1 };
public:
    MaterialTest();
    virtual void OnRender()override;
    virtual void OnImGuiRender()override;
};

