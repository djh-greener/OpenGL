#pragma once
#include "Test.h"
class AdvancedLightingTest :
    public Test
{
	bool Blinn;
public:
	AdvancedLightingTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

