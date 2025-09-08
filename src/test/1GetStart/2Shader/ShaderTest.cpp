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
	{//Default:插值顶点颜色的三角形
		float vertices[] = {
			// 位置              // 颜色
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
			 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
		};
		objects.push_back(make_shared<Object>());
		objects[0]->shader = make_shared<Shader>(GetResDir() + "res/shader/1GetStart/2Shader/ShaderTest.shader");
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(vertices, sizeof(vertices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);//位置
		objects[0]->layout->Push<float>(3);//颜色
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
	}
	{//First:修改顶点着色器让三角形上下颠倒
		objects.push_back(make_shared<Object>());
		objects[1]->shader = make_shared<Shader>(GetResDir() + "res/shader/1GetStart/2Shader/ShaderTest_UpSideDown.shader");
	}
	{//Second:使用uniform定义一个水平偏移量，在顶点着色器中使用这个偏移量把三角形移动到屏幕右侧
		objects.push_back(make_shared<Object>());
		objects[2]->shader = make_shared<Shader>(GetResDir() + "res/shader/1GetStart/2Shader/ShaderTest_MoveRight.shader");
	}
	{//Third:使用out关键字把顶点位置输出到片段着色器，并将片段的颜色设置为与顶点位置相等（来看看连顶点位置值都在三角形中被插值的结果）。
		//做完这些后，尝试回答下面的问题：为什么在三角形的左下角是黑的?
		objects.push_back(make_shared<Object>());
		objects[3]->shader = make_shared<Shader>(GetResDir() + "res/shader/1GetStart/2Shader/ShaderTest_PosByColor.shader");

	}
	{//MyTry:彩色六芒星
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
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//填充模式
		objects[0]->shader->Bind();
		objects[0]->vao->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		break;
	case First:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//填充模式
		objects[1]->shader->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		break;
	case Second:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//填充模式
		objects[2]->shader->Bind();
		objects[2]->shader->SetUniform1f("offset", 0.3);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		break;
	case Third:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//填充模式
		objects[3]->shader->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		break;
	case MyTry:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框模式
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
