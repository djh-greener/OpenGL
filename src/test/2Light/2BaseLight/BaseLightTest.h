#pragma once
#include "Test.h"
#include<unordered_map>
class Shader;
class BaseLightTest :
    public Test
{
    enum Excercise {
        Default, First, Second, Third, Fourth
    }exercise = Default;
    float ambient=0.1, diffuse=1, specular=0.5;
    int exponent = 32;
    unordered_map<Excercise, shared_ptr<Shader>>ShaderCach;
public:
    BaseLightTest();
    virtual void OnRender()override;
    virtual void OnImGuiRender()override;
};

