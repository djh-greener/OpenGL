#include "TriangleTest.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Object.h"
#include "MyDebug.h"
#include "imgui/imgui.h"
TriangleTest::TriangleTest()
{
    
    {//原教程
        float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
            };
        objects.push_back(make_shared<Object>());
        objects[0]->shader = make_shared<Shader>(GetResDir() + "res/shader/1GetStart/1Triangle/TriangleTest.shader");
        objects[0]->vao = make_shared< VertexArray>();
        objects[0]->vbo = make_shared< VertexBuffer>(vertices, sizeof(vertices));
        objects[0]->layout = make_shared< VertexBufferLayout>();
        objects[0]->layout->Push<float>(3);
        objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
    }
    
    {//练习一：添加更多顶点到数据中，使用glDrawArrays，尝试绘制两个彼此相连的三角形
        float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
         1.0f,  0.5f, 0.0f
            };
        objects.push_back(make_shared< Object>());
        objects[1]->vao = make_shared< VertexArray>();
        objects[1]->vbo = make_shared< VertexBuffer>(vertices, sizeof(vertices));
        objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[0]->layout);
    }
    
    {//练习二：创建相同的两个三角形，但对它们的数据使用不同的VAO和VBO
        float vertices1[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
        };
        float vertices2[] = {
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
         1.0f,  0.5f, 0.0f
        };
        objects.push_back(make_shared< Object>());
        objects[2]->vao=make_shared<VertexArray>();
        objects[2]->vbo = make_shared<VertexBuffer>(vertices1, sizeof(vertices1));
        objects[2]->vao->AddBuffer(*objects[2]->vbo, *objects[0]->layout);

        objects.push_back(make_shared< Object>());
        objects[3]->vao = make_shared< VertexArray>();
        objects[3]->vbo = make_shared< VertexBuffer>(vertices2, sizeof(vertices2));
        objects[3]->vao->AddBuffer(*objects[3]->vbo, *objects[0]->layout);
    }
    {//练习三：创建两个着色器程序，第二个程序使用一个不同的片段着色器，输出黄色；再次绘制这两个三角形，让其中一个输出为黄色
        float vertices1[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
        };
        float vertices2[] = {
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
         1.0f,  0.5f, 0.0f
        };
        objects.push_back(make_shared< Object>());
        objects[4]->vao = make_shared< VertexArray>();
        objects[4]->vbo = make_shared< VertexBuffer>(vertices1, sizeof(vertices1));
        objects[4]->vao->AddBuffer(*objects[4]->vbo, *objects[0]->layout);

        objects.push_back(make_shared< Object>());
        objects[5]->shader = make_shared< Shader>(GetResDir() + "res/shader/1GetStart/1Triangle/TriangleTest_yellow.shader");
        objects[5]->vao = make_shared< VertexArray>();
        objects[5]->vbo = make_shared< VertexBuffer>(vertices2, sizeof(vertices2));
        objects[5]->vao->AddBuffer(*objects[5]->vbo, *objects[0]->layout);
    }
}

void TriangleTest::OnRender()
{
    GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineWidth(2);
    switch (exercise)
    {
    case Default:
        objects[0]->shader->Bind();
        objects[0]->vao->Bind(); 

        glDrawArrays(GL_TRIANGLES, 0, 3);
        break;
    case First:
        objects[0]->shader->Bind();
        objects[1]->vao->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        break;
    case Second:
        objects[0]->shader->Bind();
        objects[2]->vao->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        objects[3]->vao->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        break;
    case Third:
        objects[0]->shader->Bind();
        objects[4]->vao->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        objects[5]->shader->Bind();
        objects[5]->vao->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        break;
    default:
        break;
    }

}

void TriangleTest::OnImGuiRender()
{
    if (ImGui::Button("Default"))
        exercise = Default;
    if (ImGui::Button("First"))
        exercise = First;
    if (ImGui::Button("Second"))
        exercise = Second;
    if (ImGui::Button("Third"))
        exercise = Third;
}
