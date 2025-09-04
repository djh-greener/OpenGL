#include "DisAppearTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include "Model.h"
struct Vertex {
	glm::vec3 Pos;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
	glm::vec3 Tangent;
	glm::vec3 BiTangent;
};

DisAppearTest::DisAppearTest()
{
	model = make_shared<Model>("res/model/nanosuit/nanosuit.obj",true);
	Shaders["Nanosuit"] = make_shared<Shader>("res/shader/7Shaders/DisAppear/Nanosuit.glsl");
	Shaders["DrawNormal"] = make_shared<Shader>("res/shader/4Advanced/9GeometryShader/DrawNormal.shader");
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
	//����Debug
	//Shaders["DrawNormal"]->Bind();
	//Shaders["DrawNormal"]->SetUniformMat4f("model", modelMat);
	//Shaders["DrawNormal"]->SetUniformMat4f("view", m_View);
	//Shaders["DrawNormal"]->SetUniformMat4f("projection", m_Proj);
	//model->DrawModel(Shaders["DrawNormal"]);

}

void DisAppearTest::OnImGuiRender()
{
}
