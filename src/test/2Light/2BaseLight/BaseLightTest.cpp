#include "BaseLightTest.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Object.h"
#include "MyDebug.h"
#include "Texture.h"
#include "imgui/imgui.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<GLFW/glfw3.h>

BaseLightTest::BaseLightTest()
{
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    {//Box
        objects.push_back(make_shared<Object>());
        ShaderCach[Default] = make_shared<Shader>(GetResDir() + "res/shader/2Light/2BaseLight/Default.shader");
        ShaderCach[Third] = make_shared<Shader>(GetResDir() + "res/shader/2Light/2BaseLight/Third.shader");
        ShaderCach[Fourth] = make_shared<Shader>(GetResDir() + "res/shader/2Light/2BaseLight/Fourth.shader");

        objects[0]->shader = ShaderCach[Default];
        objects[0]->shader->Bind();
        objects[0]->vao = make_shared<VertexArray>();
        objects[0]->vbo = make_shared<VertexBuffer>(vertices, sizeof(vertices));
        objects[0]->layout = make_shared<VertexBufferLayout>();
        objects[0]->layout->Push<float>(3);
        objects[0]->layout->Push<float>(3);
        objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
    }
    {//Light
        objects.push_back(make_shared<Object>());
        objects[1]->shader = make_shared<Shader>(GetResDir() + "res/shader/2Light/2BaseLight/light.shader");
        objects[1]->shader->Bind();
        objects[1]->pos = {1.2f, 1.0f, 2.0f};
    }
    {//Axis
        float axisVectices[] = {
        0.0f,0.0f,0.0f,
        10.f,0.f,0.f,
        0.0f,0.0f,0.0f,
        0.f,10.f,0.f,
        0.0f,0.0f,0.0f,
        0.f,0.f,10.f
        };
        objects.push_back(make_shared<Object>());

        objects[2]->shader = make_shared<Shader>(GetResDir() + "res/shader/axis.shader");
        objects[2]->shader->Bind();
        objects[2]->shader->SetUniformMat4f("model", glm::mat4(1));//默认静止，所以模型矩阵直接写死，不在渲染循环更新

        objects[2]->vao = make_shared<VertexArray>();
        objects[2]->vbo = make_shared<VertexBuffer>(axisVectices, sizeof(axisVectices));
        objects[2]->layout = make_shared<VertexBufferLayout>();
        objects[2]->layout->Push<float>(3);//位置
        objects[2]->vao->AddBuffer(*objects[2]->vbo, *objects[2]->layout);
    }
}

void BaseLightTest::OnRender()
{
    GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    switch (exercise)
    {
    case Default:case Second:
    {//尝试使用不同的环境光、漫反射和镜面强度，观察它们怎么是影响光照效果的。
        //同样，尝试实验一下镜面光照的反光度因子。尝试理解为什么某一个值能够有着某一种视觉输出。
        objects[0]->shader = ShaderCach[Default];
        objects[0]->shader->Bind();
        objects[0]->shader->SetUniform3f("viewPos", Camera::GetInstance()->Position);
        break;
    }
    case First:
    {//目前，我们的光源是静止的，你可以尝试使用sin或cos函数让光源在场景中来回移动。
        //观察光照随时间的改变能让你更容易理解冯氏光照模型
        objects[1]->pos.x= 1.2*sin(glfwGetTime());
        break;
    }
    case Third:
        //在观察空间（而不是世界空间）中计算冯氏光照
        objects[0]->shader = ShaderCach[Third];
        break;
    case Fourth:
        //尝试实现一个Gouraud着色（而不是冯氏着色）
        objects[0]->shader = ShaderCach[Fourth];
        break;
    default:
        break;
    }

    objects[0]->shader->Bind();
    objects[0]->vao->Bind();
    objects[0]->shader->SetUniformMat4f("model", glm::mat4(1.f));
    objects[0]->shader->SetUniformMat4f("view", m_View);
    objects[0]->shader->SetUniformMat4f("projection", m_Proj);
    objects[0]->shader->SetUniform3f("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    objects[0]->shader->SetUniform3f("lightColor", glm::vec3(1.0f));
    objects[0]->shader->SetUniform3f("lightPos", objects[1]->pos);
    objects[0]->shader->SetUniform1f("ambientStrength", ambient);
    objects[0]->shader->SetUniform1f("diffuseStrength", diffuse);
    objects[0]->shader->SetUniform1f("specularStrength", specular);
    objects[0]->shader->SetUniform1i("exponent", exponent);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    objects[1]->shader->Bind();
    auto model = glm::translate(glm::mat4(1.f), objects[1]->pos);
    model = glm::scale(model, glm::vec3(0.2f));
    objects[1]->shader->SetUniformMat4f("model", model);
    objects[1]->shader->SetUniformMat4f("view", m_View);
    objects[1]->shader->SetUniformMat4f("projection", m_Proj);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    objects[2]->shader->Bind();
    objects[2]->vao->Bind();
    for (int i = 0; i < 3; i++) {
        glm::vec3 tmp = { 0,0,0 };
        tmp[i] = 1;
        objects[2]->shader->SetUniform3f("objectColor", tmp);
        objects[2]->shader->SetUniformMat4f("view", m_View);
        objects[2]->shader->SetUniformMat4f("projection", m_Proj);
        GLCall(glDrawArrays(GL_LINES, 2 * i, 2));
        GLCall(glLineWidth(3));
    }
}

void BaseLightTest::OnImGuiRender()
{
    if (ImGui::Button("Default")) 
        exercise = Default;
    ImGui::SliderFloat3("lightPos", &objects[1]->pos.x, -3, 3);
    if (ImGui::Button("First"))
        exercise = First;
    if (ImGui::Button("Second"))
        exercise = Second;
    ImGui::SliderFloat("ambient", &ambient, 0, 1);
    ImGui::SliderFloat("diffuse", &diffuse, 0, 1);
    ImGui::SliderFloat("specular", &specular, 0, 1);
    ImGui::SliderInt("exponent", &exponent, 0, 128);
    if (ImGui::Button("Third"))
        exercise = Third;
    if (ImGui::Button("Fourth"))
        exercise = Fourth;
}
