#include "BlendTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"

#include"Model.h"
#include "imgui/imgui.h"

#include<map>
BlendTest::BlendTest()
{
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
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

		objects[0]->shader = make_shared<Shader>("res/shader/axis.shader");
		objects[0]->shader->Bind();
		objects[0]->shader->SetUniformMat4f("model", glm::mat4(1));//默认静止，所以模型矩阵直接写死，不在渲染循环更新
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(axisVectices, sizeof(axisVectices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);//位置
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
	}
	{//地板和正方体
		objects.push_back(make_shared<Object>());
		objects[1]->shader = make_shared<Shader>("res/shader/4Advanced/3Blend/Cube.shader");
		objects[1]->vao = make_shared<VertexArray>();
		objects[1]->vbo = make_shared<VertexBuffer>(vertices, sizeof(vertices));
		objects[1]->layout = make_shared<VertexBufferLayout>();
		objects[1]->layout->Push<float>(3);
		objects[1]->layout->Push<float>(3);
		objects[1]->layout->Push<float>(2);
		objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[1]->layout);
		objects[1]->texs.push_back(make_shared<Texture>("res/image/metal.png"));//地板的纹理
		objects[1]->texs.push_back(make_shared<Texture>("res/image/marble.jpg"));//正方体的纹理
		
	}
	float transparentVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};
	{//半透明：草
		objects.push_back(make_shared<Object>());
		objects[2]->shader = make_shared<Shader>("res/shader/4Advanced/3Blend/Grass.shader");
		objects[2]->vao = make_shared<VertexArray>();
		objects[2]->vbo = make_shared<VertexBuffer>(transparentVertices, sizeof(transparentVertices));
		objects[2]->layout = make_shared<VertexBufferLayout>();
		objects[2]->layout->Push<float>(3);
		objects[2]->layout->Push<float>(2);
		objects[2]->vao->AddBuffer(*objects[2]->vbo, *objects[2]->layout);
		objects[2]->texs.push_back(make_shared<Texture>("res/image/grass.png"));
	}
	{//半透明：窗户
		objects.push_back(make_shared<Object>());
		objects[3]->shader = make_shared<Shader>("res/shader/4Advanced/3Blend/Window.shader");
		objects[3]->texs.push_back(make_shared<Texture>("res/image/window.png"));
	}
}

void BlendTest::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ));
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
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


	objects[1]->shader->Bind();
	objects[1]->vao->Bind();
	objects[1]->texs[0]->Bind(0);
	objects[1]->texs[1]->Bind(1);
	objects[1]->shader->SetUniformMat4f("projection", m_Proj);
	objects[1]->shader->SetUniformMat4f("view", m_View);
	objects[1]->shader->SetUniform3f("viewPos", Camera::GetInstance()->Position);
	objects[1]->shader->SetUniform1f("material.shininess", 32);
	objects[1]->shader->SetUniform3f("dirLight.direction", glm::vec3(0, 1, -1));
	objects[1]->shader->SetUniform3f("dirLight.ambient", 0.5f * lightColor);
	objects[1]->shader->SetUniform3f("dirLight.diffuse", 0.8f * lightColor);
	objects[1]->shader->SetUniform3f("dirLight.specular", lightColor);
	
	//地板
	objects[1]->shader->SetUniformMat4f("model", glm::scale(glm::mat4(1), glm::vec3(10, 0.1, 10)));
	objects[1]->shader->SetUniform1i("material.diffuse1", 0);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	//正方体
	objects[1]->shader->SetUniform1i("material.diffuse1", 1);
	objects[1]->shader->SetUniformMat4f("model", glm::translate(glm::mat4(1), glm::vec3(2, 0.5 + 0.06, 2)));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	objects[1]->shader->SetUniformMat4f("model", glm::translate(glm::mat4(1), glm::vec3(-1, 0.5 + 0.06, -1)));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	//最后渲染透明物体
	if (transparent == "Grass") {
		objects[2]->shader->Bind();
		objects[2]->vao->Bind();
		objects[2]->texs[0]->Bind(2);
		objects[2]->texs[0]->SetRound(GL_CLAMP_TO_EDGE);
		objects[2]->shader->SetUniformMat4f("projection", m_Proj);
		objects[2]->shader->SetUniformMat4f("view", m_View);
		objects[2]->shader->SetUniform1i("diffuse", 2);

		vector<glm::vec3> vegetation
		{
			glm::vec3(-1.5f, 0.0f, -0.48f),
			glm::vec3(1.5f, 0.0f, 0.51f),
			glm::vec3(0.0f, 0.0f, 0.7f),
			glm::vec3(-0.3f, 0.0f, -2.3f),
			glm::vec3(0.5f, 0.0f, -0.6f)
		};
		for (int i = 0; i < vegetation.size(); i++)
		{
			auto model = glm::translate(glm::mat4(1), glm::vec3(0, 0.5 + 0.06, 0));
			model = glm::translate(model, vegetation[i]);
			objects[2]->shader->SetUniformMat4f("model", model);
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		}
	}
	if (transparent == "Window") {
		objects[3]->shader->Bind();
		objects[2]->vao->Bind();
		objects[3]->texs[0]->Bind(2);
		objects[3]->texs[0]->SetRound(GL_CLAMP_TO_EDGE);
		objects[3]->shader->SetUniformMat4f("projection", m_Proj);
		objects[3]->shader->SetUniformMat4f("view", m_View);
		objects[3]->shader->SetUniform1i("diffuse", 2);

		vector<glm::vec3> vegetation
		{
			glm::vec3(-1.5f, 0.0f, -0.48f),
			glm::vec3(1.5f, 0.0f, 0.51f),
			glm::vec3(0.0f, 0.0f, 0.7f),
			glm::vec3(-0.3f, 0.0f, -2.3f),
			glm::vec3(0.5f, 0.0f, -0.6f)
		};
		map < float, glm::vec3 > sorted;
		for (int i = 0; i < vegetation.size(); i++)
		{
			float distance = glm::length(Camera::GetInstance()->Position - vegetation[i]);
			sorted[distance] = vegetation[i];
		}
		for (auto it = sorted.rbegin(); it != sorted.rend(); it++)
		{
			auto model = glm::translate(glm::mat4(1), glm::vec3(0, 0.5 + 0.06, 0));
			model = glm::translate(model, it->second);
			objects[2]->shader->SetUniformMat4f("model", model);
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		}
	}
}

void BlendTest::OnImGuiRender()
{
	if (ImGui::Button("Grass"))
		transparent = "Grass";
	if (ImGui::Button("Window"))
		transparent = "Window";
}
