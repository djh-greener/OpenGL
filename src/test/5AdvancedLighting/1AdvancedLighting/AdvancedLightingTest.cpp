#include "AdvancedLightingTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include "imgui/imgui.h"

AdvancedLightingTest::AdvancedLightingTest()
{
	float planeVertices[] = {
		// positions            // normals         // texcoords
		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
		 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
	};
	{//µÿ∞Â
		objects.push_back(make_shared<Object>());
		objects[0]->shader = make_shared<Shader>("res/shader/5AdvancedLighting/1AdvancedLighting/Floor.shader");
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(planeVertices, sizeof(planeVertices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(2);
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
		objects[0]->texs.push_back(make_shared<Texture>("res/image/wood.png"));
	}
	GLfloat cubeVertices[] = {
		// Positions       
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};
	{//π‚‘¥
		objects.push_back(make_shared<Object>());
		objects[1]->shader = make_shared<Shader>("res/shader/light.shader");
		objects[1]->vao = make_shared<VertexArray>();
		objects[1]->vbo = make_shared<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
		objects[1]->layout = make_shared<VertexBufferLayout>();
		objects[1]->layout->Push<float>(3);
		objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[1]->layout);
	}
}

void AdvancedLightingTest::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	objects[0]->shader->Bind();
	objects[0]->vao->Bind();
	objects[0]->texs[0]->Bind(0);
	objects[0]->shader->SetUniformMat4f("model", glm::mat4(1));
	objects[0]->shader->SetUniformMat4f("view", m_View);
	objects[0]->shader->SetUniformMat4f("projection",m_Proj);
	objects[0]->shader->SetUniform3f("LightPos", objects[1]->pos);
	objects[0]->shader->SetUniform3f("ViewPos", Camera::GetInstance()->Position);
	objects[0]->shader->SetUniform1i("FloorTexture", 0);
	objects[0]->shader->SetUniform1i("Blinn",Blinn);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

	objects[1]->shader->Bind();
	objects[1]->vao->Bind();
	glm::mat4 model = glm::translate(glm::mat4(1), objects[1]->pos);
	model = glm::scale(model, glm::vec3(0.1f));

	objects[1]->shader->SetUniformMat4f("model", model);
	objects[1]->shader->SetUniformMat4f("view", m_View);
	objects[1]->shader->SetUniformMat4f("projection", m_Proj);
	objects[1]->shader->SetUniform3f("LightColor", glm::vec3(1));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}

void AdvancedLightingTest::OnImGuiRender()
{
	if (ImGui::Button("Blinn"))
		Blinn = !Blinn;
	string str = Blinn ? "ON" : "OFF";
	ImGui::SameLine(70);
	ImGui::Text("Blinn is %s", str.c_str());

	ImGui::SliderFloat3("LightPos", &objects[1]->pos.x, -10, 10);
}
