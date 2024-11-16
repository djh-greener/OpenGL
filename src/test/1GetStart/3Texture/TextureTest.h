#pragma once
#include "Test.h"
class TextureTest :
    public Test
{
    enum Exercise {
        Default, First, Second, Third, Fourth
    };
    Exercise exercise = Default;
    float factor=0.2;
public:
    TextureTest();
    virtual void OnRender()override;
    virtual void OnImGuiRender()override;
};

