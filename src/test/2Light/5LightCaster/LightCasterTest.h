#pragma once
#include "Test.h"
#include<unordered_map>
class LightCasterTest :
    public Test
{
    enum CurrentShader {
        DirectionalLight,
        PointLight,
        FlashLight
    }currentShader;
    unordered_map<CurrentShader, shared_ptr<Shader>>ShaderCach;
    glm::vec3 lightColor;

public:
    LightCasterTest();
    virtual void OnRender()override;
    virtual void OnImGuiRender()override;

};

