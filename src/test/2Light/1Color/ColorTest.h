#pragma once
#include "Test.h"
class ColorTest :
    public Test
{
public:
    ColorTest();
    virtual void OnRender()override;
    virtual void OnImGuiRender()override;
};

