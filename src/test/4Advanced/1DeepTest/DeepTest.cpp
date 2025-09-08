#include "DeepTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include"Model.h"

#include "MyDebug.h"
#include "Camera.h"
DeepTest::DeepTest()
{
	model = make_shared<Model>(GetResDir() + "res/model/nanosuit/nanosuit.obj");
	shader = make_shared<Shader>(GetResDir() + "res/shader/4Advanced/1DeepTest/Default.shader");

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

void DeepTest::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	shader->Bind();
	shader->SetUniformMat4f("model", glm::mat4(1));
	shader->SetUniformMat4f("projection", m_Proj);
	shader->SetUniformMat4f("view", m_View);
	model->DrawModel(shader);
}

void DeepTest::OnImGuiRender()
{
}
