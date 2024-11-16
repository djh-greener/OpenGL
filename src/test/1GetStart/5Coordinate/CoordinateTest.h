#pragma once
#include "Test.h"
class CoordinateTest :
    public Test
{
    enum Excercise {
        Default, First, Second,Third
    };
    Excercise exercise = Default;
    float fov=45, asp=1;
    glm::vec3 CameraPos = { 0,0,3 };
public:
    CoordinateTest();
    virtual void OnRender()override;
    virtual void OnImGuiRender()override;
    void SetMatrixDefault();
};

