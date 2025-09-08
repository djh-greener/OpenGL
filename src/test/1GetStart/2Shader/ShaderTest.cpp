#include "ShaderTest.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Object.h"
#include "MyDebug.h"
#include "imgui/imgui.h"
ShaderTest::ShaderTest()
{
	{//Default:��ֵ������ɫ��������
		float vertices[] = {
			// λ��              // ��ɫ
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
			-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
			 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
		};
		objects.push_back(make_shared<Object>());
		objects[0]->shader = make_shared<Shader>(GetResDir() + "res/shader/1GetStart/2Shader/ShaderTest.shader");
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(vertices, sizeof(vertices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);//λ��
		objects[0]->layout->Push<float>(3);//��ɫ
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
	}
	{//First:�޸Ķ�����ɫ�������������µߵ�
		objects.push_back(make_shared<Object>());
		objects[1]->shader = make_shared<Shader>(GetResDir() + "res/shader/1GetStart/2Shader/ShaderTest_UpSideDown.shader");
	}
	{//Second:ʹ��uniform����һ��ˮƽƫ�������ڶ�����ɫ����ʹ�����ƫ�������������ƶ�����Ļ�Ҳ�
		objects.push_back(make_shared<Object>());
		objects[2]->shader = make_shared<Shader>(GetResDir() + "res/shader/1GetStart/2Shader/ShaderTest_MoveRight.shader");
	}
	{//Third:ʹ��out�ؼ��ְѶ���λ�������Ƭ����ɫ��������Ƭ�ε���ɫ����Ϊ�붥��λ����ȣ�������������λ��ֵ�����������б���ֵ�Ľ������
		//������Щ�󣬳��Իش���������⣺Ϊʲô�������ε����½��Ǻڵ�?
		objects.push_back(make_shared<Object>());
		objects[3]->shader = make_shared<Shader>(GetResDir() + "res/shader/1GetStart/2Shader/ShaderTest_PosByColor.shader");

	}
	{//MyTry:��ɫ��â��
		float tmp = pow(3, 0.5) / 2;
		float vertices[] = {
		0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		tmp, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-tmp, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		tmp, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-tmp, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		};

		objects.push_back(make_shared< Object>());
		objects[4]->shader = make_shared< Shader>(GetResDir() + "res/shader/1GetStart/2Shader/ShaderTest_MyTry.shader");
		objects[4]->vao = make_shared< VertexArray>();
		objects[4]->vbo = make_shared< VertexBuffer>(vertices, sizeof(vertices));
		objects[4]->vao->AddBuffer(*objects[4]->vbo, *objects[0]->layout);
	}
}


void ShaderTest::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
	
	switch (exercise)
	{
	case Default:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//���ģʽ
		objects[0]->shader->Bind();
		objects[0]->vao->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		break;
	case First:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//���ģʽ
		objects[1]->shader->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		break;
	case Second:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//���ģʽ
		objects[2]->shader->Bind();
		objects[2]->shader->SetUniform1f("offset", 0.3);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		break;
	case Third:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//���ģʽ
		objects[3]->shader->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		break;
	case MyTry:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�߿�ģʽ
		objects[4]->shader->Bind();
		objects[4]->vao->Bind();
		glm::vec3 colorfactor(factor[0], factor[1], factor[2]);
		objects[4]->shader->SetUniform3f("factor", colorfactor);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		break;
	default:
		break;
	}
}

void ShaderTest::OnImGuiRender()
{
	if (ImGui::Button("Default"))
		exercise = Default;
	if (ImGui::Button("First"))
		exercise = First;
	if (ImGui::Button("Second"))
		exercise = Second;
	if (ImGui::Button("Third"))
		exercise = Third;
	if (ImGui::Button("MyTry"))
		exercise = MyTry;
	ImGui::ColorEdit3("color", factor);
}
