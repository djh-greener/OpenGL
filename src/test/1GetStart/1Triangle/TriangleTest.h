#pragma once
#include "Test.h"
class TriangleTest :public Test
{
	enum Exercise {
		Default,First,Second,Third
	};
	Exercise exercise = Default;
public:
	TriangleTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

