#pragma once
#include "Test.h"
class AntiAliasingTest :
    public Test
{
	bool EnableAntiAliasing = false;
	
public:
	AntiAliasingTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

