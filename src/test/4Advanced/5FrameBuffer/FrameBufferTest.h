#pragma once
#include "Test.h"
#include<memory>
#include<unordered_map>
using namespace std;
class FrameBuffer;
class Shader;
class FrameBufferTest :
    public Test
{
	shared_ptr<FrameBuffer>fbo;
	unordered_map<string, shared_ptr<Shader>>shaders;
	string CurrentShader = "Quad";
public:
	FrameBufferTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

