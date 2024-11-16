#pragma once
#include "Test.h"
class ShaderTest :public Test
{
    float factor[3] = { 0 };
    enum Exercise {
        Default, First, Second, Third, MyTry
    };
    Exercise exercise = Default;
public:
    ShaderTest();

    virtual void OnRender()override;
    virtual void OnImGuiRender()override;
};

