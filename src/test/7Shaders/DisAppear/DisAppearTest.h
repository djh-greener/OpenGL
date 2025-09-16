#pragma once
#include "Test.h"

class Model;
class Shader;
class FrameBuffer;
class ParticleSystem;
class VertexArray;
class DisAppearTest :
    public Test
{
	shared_ptr<Model>model;
	unordered_map<string, shared_ptr<Shader>>Shaders;
    shared_ptr<FrameBuffer> fbo;
    shared_ptr<ParticleSystem> particleSystem;
    shared_ptr<VertexArray> vao;

public:
	DisAppearTest();
    void OnUpdate(float deltaTime) override;
	void OnRender()override;
	void OnImGuiRender()override;
};

