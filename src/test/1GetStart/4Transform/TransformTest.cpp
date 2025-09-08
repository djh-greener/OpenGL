#include "TransformTest.h"

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

void PrintMat4(glm::mat4 mat) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << mat[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
void PrintVec4(glm::vec4 vec) {
    for (int i = 0; i < 4; i++) 
        std::cout << vec[i] << " ";
    
    std::cout << std::endl;
}
TransformTest::TransformTest()
{
    float vertices[] = {
        //     ---- λ�� ----  - �������� -
             0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // ����
             0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // ����
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // ����
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // ����
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,    // ����
             0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // ����
    };

    {//Default
        objects.push_back(make_shared<Object>());
        objects[0]->shader = make_shared<Shader>(GetResDir() + "res/shader/1GetStart/4Transform/TransformTest.shader");
        objects[0]->shader->Bind();
        objects[0]->shader->SetUniform1i("Texture0", 0);//������ɫ����Ĳ�������Ӧ������Ԫ
        objects[0]->shader->SetUniform1i("Texture1", 1);
        objects[0]->vao = make_shared<VertexArray>();
        objects[0]->vbo = make_shared<VertexBuffer>(vertices, sizeof(vertices));
        objects[0]->layout = make_shared<VertexBufferLayout>();
        objects[0]->layout->Push<float>(3);//λ��
        objects[0]->layout->Push<float>(2);//��������
        objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
        objects[0]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/container.jpg"));
        objects[0]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/awesomeface.png"));
        //���������󶨵���Ӧ����Ԫ
        for (int i = 0; i < objects[0]->texs.size(); i++)
            objects[0]->texs[i]->Bind(i);
    }
    {//First:ʹ��Ӧ���������ϵ����һ���任�����Խ���ı�Ϊ����ת����λ��
        //����Ⱦѭ��
    }
    {//Second: �����ڶ������ӣ�ֻʹ�ñ任����ڷ��ڲ�ͬ��λ�á���������ӱ��ڷ��ڴ��ڵ����Ͻǣ����һ᲻�ϵ����ţ���������ת��
        //����Ⱦѭ��
    }
}

void TransformTest::OnRender()
{
    GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    switch (exercise)
    {
    case Default: {
        objects[0]->shader->Bind();
        objects[0]->vao->Bind();

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        objects[0]->shader->SetUniformMat4f("transform", trans);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        break;
    }
    case First:
    {
        objects[0]->shader->Bind();
        objects[0]->vao->Bind();

        glm::mat4 trans = glm::mat4(1.0f);

        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        objects[0]->shader->SetUniformMat4f("transform", trans);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        break;
    }
    case Second:
        objects[0]->shader->Bind();
        objects[0]->vao->Bind();

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        float time = (float)glfwGetTime();
        trans = glm::scale(trans, glm::vec3(sin(time)/2+0.5));
        objects[0]->shader->SetUniformMat4f("transform", trans);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        break;
    }
}

void TransformTest::OnImGuiRender()
{
    if (ImGui::Button("Default"))
        exercise = Default;
    if (ImGui::Button("First"))
        exercise = First;
    if (ImGui::Button("Second"))
        exercise = Second;
}
