#pragma once
#include "Test.h"
#include <memory>
using namespace std;

class Model;
class Shader;
class DeepTest :public Test
{
	shared_ptr<Model>model;
	shared_ptr<Shader>shader;

public:
	DeepTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

