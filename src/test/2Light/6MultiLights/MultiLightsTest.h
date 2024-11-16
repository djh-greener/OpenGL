#pragma once
#include "Test.h"
class MultiLightsTest :
    public Test
{
    glm::vec3 lightColor;

    glm::vec3 dirLightdir;

    glm::vec3 pointLightPositions[4]= {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
    };
public:
    MultiLightsTest();
    virtual void OnRender()override;
    virtual void OnImGuiRender()override;
};

