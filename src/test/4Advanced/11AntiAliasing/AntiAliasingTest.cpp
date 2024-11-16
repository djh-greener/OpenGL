#include "AntiAliasingTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"

#include "imgui/imgui.h"

AntiAliasingTest::AntiAliasingTest()
{
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
	{
		objects.push_back(make_shared<Object>());
		objects[0]->shader = make_shared<Shader>("res/shader/4Advanced/11AntiAliasing/Cube.shader");
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(cubeVertices,sizeof(cubeVertices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
	}
}

void AntiAliasingTest::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	if (EnableAntiAliasing) {
		GLCall(glEnable(GL_MULTISAMPLE));
	}
	else {
		GLCall(glDisable(GL_MULTISAMPLE));
	}
		


	objects[0]->shader->Bind();
	objects[0]->vao->Bind();
	float angle = glm::radians( glfwGetTime() * 10);
	auto model = glm::mat4(1);
	//实际的顺序是反的，即先平移，后旋转
	model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
	model = glm::translate(model,  glm::vec3(3, 0, 0));

	objects[0]->shader->SetUniformMat4f("model", model);
	objects[0]->shader->SetUniformMat4f("view", m_View);
	objects[0]->shader->SetUniformMat4f("projection", m_Proj);

	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}

void AntiAliasingTest::OnImGuiRender()
{
	if (ImGui::Button("AntiAliasing"))
		EnableAntiAliasing = true;
	if (ImGui::Button("UnAntiAliasing"))
		EnableAntiAliasing = false;

}
