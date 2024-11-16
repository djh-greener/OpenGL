#include "CoordinateTest.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Object.h"
#include "MyDebug.h"
#include "Texture.h"
#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<GLFW/glfw3.h>
CoordinateTest::CoordinateTest()
{
    float vertices[] = {
     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    {//Default
        objects.push_back(make_shared<Object>());

        objects[0]->shader = make_shared<Shader>("res/shader/1GetStart/5Coordinate/CoordinateTest.shader");
        objects[0]->shader->Bind();
        objects[0]->shader->SetUniform1i("Texture0", 0);//������ɫ����Ĳ�������Ӧ������Ԫ
        objects[0]->shader->SetUniform1i("Texture1", 1);
        objects[0]->shader->SetUniformMat4f("view", m_View);
        objects[0]->shader->SetUniformMat4f("projection", m_Proj);

        objects[0]->vao = make_shared<VertexArray>();
        objects[0]->vbo = make_shared<VertexBuffer>(vertices, sizeof(vertices));
        objects[0]->layout = make_shared<VertexBufferLayout>();
        objects[0]->layout->Push<float>(3);//λ��
        objects[0]->layout->Push<float>(2);//��������
        objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
        objects[0]->texs.push_back(make_shared<Texture>("res/image/container.jpg"));
        objects[0]->texs.push_back(make_shared<Texture>("res/image/awesomeface.png"));
        //���������󶨵���Ӧ����Ԫ
        for (int i = 0; i < objects[0]->texs.size(); i++)
            objects[0]->texs[i]->Bind(i);
    }
    {//axis
        float axisVectices[] = {
        0.0f,0.0f,0.0f,
        10.f,0.f,0.f,
        0.0f,0.0f,0.0f,
        0.f,10.f,0.f,
        0.0f,0.0f,0.0f,
        0.f,0.f,10.f
        };
        objects.push_back(make_shared<Object>());

        objects[1]->shader = make_shared<Shader>("res/shader/axis.shader");
        objects[1]->shader->Bind();
        objects[1]->pos = { 0,0,0 };
        glm::mat4 model = glm::translate(glm::mat4(1), objects[0]->pos);
        objects[1]->shader->SetUniformMat4f("model", model);
        objects[1]->shader->SetUniformMat4f("view", m_View);
        objects[1]->shader->SetUniformMat4f("projection", m_Proj);
                    
        objects[1]->vao = make_shared<VertexArray>();
        objects[1]->vbo = make_shared<VertexBuffer>(axisVectices, sizeof(axisVectices));
        objects[1]->layout = make_shared<VertexBufferLayout>();
        objects[1]->layout->Push<float>(3);//λ��
        objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[1]->layout);
    }
}

void CoordinateTest::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT));
  
    objects[1]->shader->Bind();
    objects[1]->vao->Bind();
    objects[1]->shader->SetUniformMat4f("projection", m_Proj);
    objects[1]->shader->SetUniformMat4f("view", m_View);
    for (int i = 0; i < 3; i++) {
        glm::vec3 tmp = { 0,0,0 };
        tmp[i] = 1;
        objects[1]->shader->SetUniform3f("objectColor", tmp);
        GLCall(glDrawArrays(GL_LINES, 2 * i, 2));
        GLCall(glLineWidth(3));
    }
    glm::vec3 cubePositions[] = {
         glm::vec3(0.0f,  0.0f,  0.0f),
         glm::vec3(2.0f,  5.0f, -15.0f),
         glm::vec3(-1.5f, -2.2f, -2.5f),
         glm::vec3(-3.8f, -2.0f, -12.3f),
         glm::vec3(2.4f, -0.4f, -3.5f),
         glm::vec3(-1.7f,  3.0f, -7.5f),
         glm::vec3(1.3f, -2.0f, -2.5f),
         glm::vec3(1.5f,  2.0f, -2.5f),
         glm::vec3(1.5f,  0.2f, -1.5f),
         glm::vec3(-1.3f,  1.0f, -1.5f)
    };
	switch (exercise)
	{
	case Default:
	{
		objects[0]->shader->Bind();
		objects[0]->vao->Bind();
		objects[0]->shader->SetUniformMat4f("projection", m_Proj);//ͶӰ������ΪĬ��ֵ
		objects[0]->shader->SetUniformMat4f("view", m_View);//�۲������ΪĬ��ֵ
		for (int i = 0; i < 10; i++) {
			//ʵ�ʵı任˳��������ת��ƽ��
			glm::mat4 trans = glm::translate(glm::mat4(1), cubePositions[i]);
			float angle = 20 * i;
			glm::mat4 model = glm::rotate(trans, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
			objects[0]->shader->SetUniformMat4f("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		break;
	}
	case First://��GLM��projection�����е�FoV��aspect-ratio��������ʵ�顣���ܷ�㶮���������Ӱ��͸��ƽ��ͷ���
	{
		objects[0]->shader->Bind();
		objects[0]->vao->Bind();
		m_Proj = glm::perspective(fov, asp, 0.1f, 100.0f);
		objects[0]->shader->SetUniformMat4f("model", glm::rotate(glm::mat4(1), glm::radians(50.f), glm::vec3(0.5f, 1.0f, 0.0f)));
		objects[0]->shader->SetUniformMat4f("projection", m_Proj);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		break;
	}
	case Second:
	{//���۲�����ڸ��������Ͻ���λ�ƣ���������������θı�ġ�ע��ѹ۲���󵱳����������
		objects[0]->shader->Bind();
		objects[0]->vao->Bind();
		objects[0]->shader->SetUniformMat4f("model", glm::rotate(glm::mat4(1), glm::radians(50.f), glm::vec3(0.5f, 1.0f, 0.0f)));
		objects[0]->shader->SetUniformMat4f("projection", m_Proj);
		m_View = glm::translate(glm::mat4(1.0f), -CameraPos);
		objects[0]->shader->SetUniformMat4f("view", m_View);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		break;
	}
	case Third:
	{//ʹ��ģ�;���ֻ����3������������ת���Լ���1�����ӣ�������ʣ�µ����ӱ��־�ֹ
        objects[0]->shader->Bind();
        objects[0]->vao->Bind();
        objects[0]->shader->SetUniformMat4f("projection", m_Proj);//ͶӰ������ΪĬ��ֵ
        objects[0]->shader->SetUniformMat4f("view", m_View);//�۲������ΪĬ��ֵ
        for (int i = 0; i < 10; i++) {
            //ʵ�ʵı任˳��������ת��ƽ��
            glm::mat4 trans = glm::translate(glm::mat4(1), cubePositions[i]);
            float angle;
            if (i % 3 == 0)
                angle = 20 * i;
            else
                angle = 0;
            glm::mat4 model = glm::rotate(trans, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            objects[0]->shader->SetUniformMat4f("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        break;
	}
	}
}

void CoordinateTest::OnImGuiRender()
{
    if (ImGui::Button("Default")) {
        SetMatrixDefault();
        exercise = Default;
    }
        
    if (ImGui::Button("First"))
    {
        exercise = First;
        SetMatrixDefault();
    }

    ImGui::SliderFloat("FoV", &fov, 40, 50);
    ImGui::SliderFloat("aspect", &asp, 0.1, 10);

    if (ImGui::Button("Second")) {
        exercise = Second;
        SetMatrixDefault();
    }
    ImGui::SliderFloat3("CameraPos", &CameraPos.x, -5, 5);

    if (ImGui::Button("Third")) {
        exercise = Third;
        SetMatrixDefault();
    }
    
}

void CoordinateTest::SetMatrixDefault()
{
    fov = 45;
    asp = 1;
    m_Proj = glm::perspective(fov, asp, 0.1f, 100.0f);

    CameraPos = { 0,0,3 };
    m_View=glm::translate(glm::mat4(1.0f), -CameraPos);

}


