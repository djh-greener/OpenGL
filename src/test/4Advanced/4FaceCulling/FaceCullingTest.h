#pragma once
#include "Test.h"
class FaceCullingTest :
    public Test
{
public:
	FaceCullingTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

