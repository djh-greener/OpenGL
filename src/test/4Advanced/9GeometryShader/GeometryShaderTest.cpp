#include "GeometryShaderTest.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include"Model.h"

#include "imgui/imgui.h"
GeometryShaderTest::GeometryShaderTest()
{
	float points[] = {
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
	-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
	};

	{
		objects.push_back(make_shared<Object>());
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(points, sizeof(points));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(2);
		objects[0]->layout->Push<float>(3);
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
	}
	model = make_shared<Model>(GetResDir() + "res/model/nanosuit/nanosuit.obj");
	{//生成一系列着色器
		vector<string>shadernames = { "Point","Line","House", "Explode","DrawNormal"};
		string directory = GetResDir() + "res/shader/4Advanced/9GeometryShader/";
		for (auto name : shadernames) {
			shaders[name] = make_shared<Shader>(directory + name + ".shader");
		}
		//辅助"DrawNormal"的，不可选
		shaders["Nanosuit"] = make_shared<Shader>(GetResDir() + "res/shader/Nanosuit.shader");
	}
}

void GeometryShaderTest::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	

	if (CurrentShaderName == "Explode"  ) {
		shaders[CurrentShaderName]->Bind();
		shaders[CurrentShaderName]->SetUniformMat4f("model", glm::mat4(1));
		shaders[CurrentShaderName]->SetUniformMat4f("projection", m_Proj);
		shaders[CurrentShaderName]->SetUniformMat4f("view", m_View);
		shaders[CurrentShaderName]->SetUniform1f("time", glfwGetTime());

		model->DrawModel(shaders[CurrentShaderName]);
	}
	else if (CurrentShaderName == "DrawNormal") {
		shaders["Nanosuit"]->Bind();
		shaders["Nanosuit"]->SetUniformMat4f("model", glm::mat4(1));
		shaders["Nanosuit"]->SetUniformMat4f("projection", m_Proj);
		shaders["Nanosuit"]->SetUniformMat4f("view", m_View);
		model->DrawModel(shaders["Nanosuit"]);
		
		shaders["DrawNormal"]->Bind();
		shaders["DrawNormal"]->SetUniformMat4f("model", glm::mat4(1));
		shaders["DrawNormal"]->SetUniformMat4f("projection", m_Proj);
		shaders["DrawNormal"]->SetUniformMat4f("view", m_View);
		model->DrawModel(shaders["DrawNormal"]);
	}
		
	else
	{
		shaders[CurrentShaderName]->Bind();
		objects[0]->vao->Bind();
		GLCall(glDrawArrays(GL_POINTS, 0, 4));
	}

}

void GeometryShaderTest::OnImGuiRender()
{
	if (ImGui::Button("Point"))
		CurrentShaderName = "Point";
	if (ImGui::Button("Line"))
		CurrentShaderName = "Line";
	if (ImGui::Button("House"))
		CurrentShaderName = "House";
	if (ImGui::Button("Explode"))
		CurrentShaderName = "Explode";
	if (ImGui::Button("DrawNormal"))
		CurrentShaderName = "DrawNormal";
}
