#include "AdvancedGLSLTest.h"
#include "UniformBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"

#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
AdvancedGLSLTest::AdvancedGLSLTest()
{
	float cubeVertices[] = {
		// positions         
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
		-0.5f,  0.5f, -0.5f,
	};
	{
		objects.push_back(make_shared<Object>());
		objects[0]->shader = make_shared<Shader>("res/shader/4Advanced/8AdvancedGLSL/ColorfulCube.shader");
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
		UniformBuffer uniformBuffer = UniformBuffer(0, 2 * sizeof(glm::mat4));
		uniformBuffer.AddBuffer(0, sizeof(glm::mat4), glm::value_ptr(m_View));
		uniformBuffer.AddBuffer(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_Proj));
	}
}

void AdvancedGLSLTest::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	objects[0]->shader->Bind();
	objects[0]->vao->Bind();
	for (int i = 0; i < positions.size(); i++) {
		objects[0]->shader->SetUniformMat4f("model", glm::translate(glm::mat4(1),positions[i]));
		glm::vec4 tmp = glm::vec4(0);
		tmp[i] = 1;
		objects[0]->shader->SetUniform3f("Color", glm::vec3(tmp));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}
}

void AdvancedGLSLTest::OnImGuiRender()
{
}
