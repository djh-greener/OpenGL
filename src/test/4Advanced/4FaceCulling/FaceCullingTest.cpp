#include "FaceCullingTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"

#include "imgui/imgui.h"
FaceCullingTest::FaceCullingTest()
{
	float cubeVertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		// Right face
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		// Top face
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
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

		objects[0]->shader = make_shared<Shader>(GetResDir() + "res/shader/axis.shader");
		objects[0]->shader->Bind();
		objects[0]->shader->SetUniformMat4f("model", glm::mat4(1));//默认静止，所以模型矩阵直接写死，不在渲染循环更新
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(axisVectices, sizeof(axisVectices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);//位置
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
	}
	{//正方体
		objects.push_back(make_shared<Object>());
		objects[1]->shader = make_shared<Shader>(GetResDir() + "res/shader/4Advanced/4FaceCulling/Cube.shader");
		objects[1]->vao = make_shared<VertexArray>();
		objects[1]->vbo = make_shared<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
		objects[1]->layout = make_shared<VertexBufferLayout>();
		objects[1]->layout->Push<float>(3);
		objects[1]->layout->Push<float>(2);
		objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[1]->layout);
		objects[1]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/marble.jpg"));//正方体的纹理

	}
}

void FaceCullingTest::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glEnable(GL_CULL_FACE));
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
	objects[1]->shader->SetUniformMat4f("projection", m_Proj);
	objects[1]->shader->SetUniformMat4f("view", m_View);
	objects[1]->shader->SetUniform1i("diffuse", 0);
	objects[1]->shader->SetUniformMat4f("model", glm::translate(glm::mat4(1), glm::vec3(2, 0.5 + 0.06, 2)));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	objects[1]->shader->SetUniformMat4f("model", glm::translate(glm::mat4(1), glm::vec3(-1, 0.5 + 0.06, -1)));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}

void FaceCullingTest::OnImGuiRender()
{
}
