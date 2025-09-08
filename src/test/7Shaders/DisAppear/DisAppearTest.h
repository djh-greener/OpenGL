#pragma once
#include "Test.h"

class Model;
class Shader;
class Vertex;
class ParticleSystem;
class DisAppearTest :
    public Test
{
	shared_ptr<Model>model;
	unordered_map<string, shared_ptr<Shader>>Shaders;
    shared_ptr<ParticleSystem> particleSystem;
public:
	DisAppearTest();
    void OnUpdate(float deltaTime) override;
	void OnRender()override;
	void OnImGuiRender()override;
};

