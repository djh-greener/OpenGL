#pragma once
#include "Test.h"
class GammaCorrectionTest :
    public Test
{
	unsigned int FloorTexture;
	unsigned int FloorTextureGamma;
	bool GammaLoad;
	bool GammaCorrect;
public:
	GammaCorrectionTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

