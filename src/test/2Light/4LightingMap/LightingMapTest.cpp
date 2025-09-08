#include "LightingMapTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"

#include "imgui/imgui.h"
#include<GLFW/glfw3.h>


LightingMapTest::LightingMapTest()
{
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    
    ShaderCach["Default"] = make_shared<Shader>(GetResDir() + "res/shader/2Light/4LightingMap/Default.shader");
    ShaderCach["Second"] = make_shared<Shader>(GetResDir() + "res/shader/2Light/4LightingMap/FlipSpecular.shader");
    ShaderCach["Third"] = ShaderCach["Default"];


    {//物体
        objects.push_back(make_shared<Object>());
        objects[0]->vao = make_shared<VertexArray>();
        objects[0]->vbo = make_shared<VertexBuffer>(vertices, sizeof(vertices));
        objects[0]->layout = make_shared<VertexBufferLayout>();
        objects[0]->layout->Push<float>(3);
        objects[0]->layout->Push<float>(3);
        objects[0]->layout->Push<float>(2);
        objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
        objects[0]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/container2.png"));
        objects[0]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/container2_specular.png"));
        objects[0]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/lighting_maps_specular_color.png"));

        for (int i = 0; i < objects[0]->texs.size(); i++)
            objects[0]->texs[i]->Bind(i);
    }
    {//光源
        objects.push_back(make_shared<Object>());
        objects[1]->shader = make_shared<Shader>(GetResDir() + "res/shader/2Light/4LightingMap/light.shader");
        objects[1]->pos = { 1.2f, 1.0f, 2.0f };
        //用的是物体的顶点数据，不过没有用上纹理坐标
    }
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

void LightingMapTest::OnRender()
{
    GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


    objects[0]->shader = ShaderCach[exercise];

    objects[0]->shader->Bind();
    objects[0]->vao->Bind();
    objects[0]->shader->SetUniformMat4f("model", glm::mat4(1));
    objects[0]->shader->SetUniformMat4f("view", m_View);
    objects[0]->shader->SetUniformMat4f("projection", m_Proj);
    objects[0]->shader->SetUniform1i("material.diffuse", 0);
    objects[0]->shader->SetUniform1i("material.specular", (exercise == "Third") + 1);
    objects[0]->shader->SetUniform1f("material.shininess", 32);
    objects[0]->shader->SetUniform3f("light.position", objects[1]->pos);
    objects[0]->shader->SetUniform3f("light.ambient", lightColor.ambient);
    objects[0]->shader->SetUniform3f("light.diffuse", lightColor.diffuse);
    objects[0]->shader->SetUniform3f("light.specular", lightColor.specular);
    objects[0]->shader->SetUniform3f("viewPos", Camera::GetInstance()->Position);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    objects[1]->shader->Bind();
    objects[0]->vao->Bind();
    objects[1]->pos.x = 1.2 * sin(glfwGetTime());
    auto model = glm::translate(glm::mat4(1.f), objects[1]->pos);
    model = glm::scale(model, glm::vec3(0.2f));
    objects[1]->shader->SetUniformMat4f("model", model);
    objects[1]->shader->SetUniformMat4f("view", m_View);
    objects[1]->shader->SetUniformMat4f("projection", m_Proj);
    objects[1]->shader->SetUniform3f("LightColor", lightColor.diffuse);
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

void LightingMapTest::OnImGuiRender()
{
    if (ImGui::Button("Default"))
        exercise = "Default";

    ImGui::Text("First");
    ImGui::ColorEdit3("Light.ambient", &lightColor.ambient.x, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs);
    ImGui::ColorEdit3("Light.diffuse", &lightColor.diffuse.x, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs);
    ImGui::ColorEdit3("Light.specular", &lightColor.specular.x, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs);

    if (ImGui::Button("Second"))
        exercise = "Second";
    if (ImGui::Button("Third"))
        exercise = "Third";
    
}
