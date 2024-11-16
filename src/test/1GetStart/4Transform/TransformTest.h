#pragma once
#include "Test.h"
class TransformTest :
    public Test
{
    enum Excercise {
        Default,First,Second
    };
    Excercise exercise = Default;
public:
    TransformTest();
    virtual void OnRender()override;
    virtual void OnImGuiRender()override;

};

