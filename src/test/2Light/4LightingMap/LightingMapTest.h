#pragma once
#include "Test.h"
#include<unordered_map>
struct LightColor
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    LightColor() {
        ambient = glm::vec3(1);
        diffuse = glm::vec3(1);
        specular = glm::vec3(1);;
    }
};

class LightingMapTest :
    public Test
{
    LightColor lightColor;
    string exercise = "Default";
    unordered_map<string, shared_ptr<Shader>>ShaderCach;
public:
    LightingMapTest();
    virtual void OnRender()override;
    virtual void OnImGuiRender()override;
};

