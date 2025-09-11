#include "DisAppearTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include "Model.h"
#include "ParticleSystem.h"
#include "ComputeShader.h"

DisAppearTest::DisAppearTest()
{
    glEnable(GL_PROGRAM_POINT_SIZE);
	model = make_shared<Model>(GetResDir() + "res/model/nanosuit/nanosuit.obj",true);
	Shaders["Nanosuit"] = make_shared<Shader>(GetResDir() + "res/shader/7Shaders/DisAppear/Nanosuit.glsl");
    Shaders["Particle"] = make_shared<Shader>( GetResDir() + "res/shader/7Shaders/DisAppear/ParticleRender.glsl");
    auto computeShader = make_shared<ComputeShader>(GetResDir() + "res/shader/7Shaders/DisAppear/ParticleCompute.glsl");
    particleSystem = make_shared<ParticleSystem>(1000, Shaders["Particle"], computeShader);
}

void DisAppearTest::OnRender()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
	glm::mat4 modelMat = glm::mat4(1);
	const float cycleDuration = 8.f;
	float time = fmod(glfwGetTime(), cycleDuration)/ cycleDuration;

	Shaders["Nanosuit"]->Bind();
	modelMat = glm::mat4(1);
	modelMat = glm::translate(modelMat, glm::vec3(4, 1, 0));
	Shaders["Nanosuit"]->SetUniformMat4f("model", modelMat);
	Shaders["Nanosuit"]->SetUniformMat4f("view", m_View);
	Shaders["Nanosuit"]->SetUniformMat4f("projection", m_Proj);
	Shaders["Nanosuit"]->SetUniform3f("LightDir", glm::vec3(0, 0, -1));
	Shaders["Nanosuit"]->SetUniform3f("ViewPos", Camera::GetInstance()->Position);
	Shaders["Nanosuit"]->SetUniform3f("minVertex", modelMat * glm::vec4(model->minVertex,1.f));
	Shaders["Nanosuit"]->SetUniform3f("maxVertex", modelMat * glm::vec4(model->maxVertex,1.f));
	Shaders["Nanosuit"]->SetUniform1f("progress", time);
	model->DrawModel(Shaders["Nanosuit"]);

    particleSystem->Render(m_View, m_Proj, false);

}

void DisAppearTest::OnImGuiRender()
{
}

void DisAppearTest::OnUpdate(float deltaTime) {
    particleSystem->Update(deltaTime, static_cast<float>(glfwGetTime()));
}
