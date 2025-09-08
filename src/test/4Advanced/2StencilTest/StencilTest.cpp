#include "StencilTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include"Model.h"

#include "MyDebug.h"
#include "Camera.h"
#include "imgui/imgui.h"
StencilTest::StencilTest()
{
	model = make_shared<Model>(GetResDir() + "res/model/nanosuit/nanosuit.obj");
	shader = make_shared<Shader>(GetResDir() + "res/shader/4Advanced/2StencilTest/Default.shader");
	edge_shader = make_shared<Shader>(GetResDir() + "res/shader/4Advanced/2StencilTest/Edge.shader");
	{//轴
		float axisVectices[] = {
			0.0f,0.0f,0.0f,
			10.f,0.f,0.f,
			0.0f,0.0f,0.0f,
			0.f,10.f,0.f,
			0.0f,0.0f,0.0f,
			0.f,0.f,10.f
		};
		objects.push_back(make_shared<Object>());

		objects[0]->shader = make_shared<Shader>(GetResDir() + "res/shader/axis.shader");
		objects[0]->shader->Bind();
		objects[0]->shader->SetUniformMat4f("model", glm::mat4(1));//默认静止，所以模型矩阵直接写死，不在渲染循环更新
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(axisVectices, sizeof(axisVectices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);//位置
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
	}
}

void StencilTest::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_STENCIL_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

	shader->Bind();
	shader->SetUniformMat4f("model", glm::mat4(1));
	shader->SetUniformMat4f("projection", m_Proj);
	shader->SetUniformMat4f("view", m_View);
	shader->SetUniform3f("viewPos", Camera::GetInstance()->Position);
	shader->SetUniform1f("material.shininess", 32);
	shader->SetUniform3f("dirLight.direction", glm::vec3(0,1,1));
	shader->SetUniform3f("dirLight.ambient", 0.5f * lightColor);
	shader->SetUniform3f("dirLight.diffuse", 0.8f * lightColor);
	shader->SetUniform3f("dirLight.specular", lightColor);


	GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));//模板和深度测试通过时，更新模板缓冲
	GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
	model->DrawModel(shader);

	GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
	GLCall(glStencilMask(0x00));//禁止更新模板缓冲
	GLCall(glDisable(GL_DEPTH_TEST));//绘制在最上层
	edge_shader->Bind();
	edge_shader->SetUniformMat4f("model", glm::scale(glm::mat4(1), glm::vec3(1.0f)));
	edge_shader->SetUniformMat4f("projection", m_Proj);
	edge_shader->SetUniformMat4f("view", m_View);
	edge_shader->SetUniform3f("EdgeColor", edgeColor);

	model->DrawModel(edge_shader);
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glStencilMask(0xFF));


	objects[0]->shader->Bind();
	objects[0]->vao->Bind();
	for (int i = 0; i < 3; i++) {
		glm::vec3 tmp = { 0,0,0 };
		tmp[i] = 1;
		objects[0]->shader->SetUniform3f("objectColor", tmp);
		objects[0]->shader->SetUniformMat4f("view", m_View);
		objects[0]->shader->SetUniformMat4f("projection", m_Proj);
		GLCall(glDrawArrays(GL_LINES, 2 * i, 2));
		GLCall(glLineWidth(3));
	}
	GLCall(glDisable(GL_STENCIL_TEST));
}

void StencilTest::OnImGuiRender()
{
	ImGui::ColorEdit3("LightColor", &lightColor.x, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs);
	ImGui::ColorEdit3("EdgeColor", &edgeColor.x, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs);
}
