#include "MaterialTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "MyDebug.h"
#include "Camera.h"

#include "imgui/imgui.h"
#include<GLFW/glfw3.h>

unordered_map<string, Material>MaterialsTable =
{
    {"emerald", {glm::vec3(0.02f, 0.17f, 0.02f), glm::vec3(0.08f, 0.61f, 0.08f), glm::vec3(0.63f, 0.73f, 0.63f), 0.6f}},
    {"jade", {glm::vec3(0.14f, 0.22f, 0.16f), glm::vec3(0.54f, 0.89f, 0.63f), glm::vec3(0.32f, 0.32f, 0.32f), 0.1f}},
    {"obsidian", {glm::vec3(0.05f, 0.05f, 0.07f), glm::vec3(0.18f, 0.17f, 0.23f), glm::vec3(0.33f, 0.33f, 0.35f), 0.3f}},
    {"pearl", {glm::vec3(0.25f, 0.21f, 0.21f), glm::vec3(1.0f, 0.83f, 0.83f), glm::vec3(0.30f, 0.30f, 0.30f), 0.09f}},
    {"ruby", {glm::vec3(0.17f, 0.01f, 0.01f), glm::vec3(0.61f, 0.04f, 0.04f), glm::vec3(0.73f, 0.63f, 0.63f), 0.6f}},
    {"turquoise", {glm::vec3(0.10f, 0.19f, 0.17f), glm::vec3(0.40f, 0.74f, 0.69f), glm::vec3(0.30f, 0.31f, 0.31f), 0.1f}},
    {"brass", {glm::vec3(0.33f, 0.22f, 0.03f), glm::vec3(0.78f, 0.57f, 0.11f), glm::vec3(0.99f, 0.94f, 0.81f), 0.22f}},
    {"bronze", {glm::vec3(0.21f, 0.12f, 0.05f), glm::vec3(0.71f, 0.43f, 0.18f), glm::vec3(0.39f, 0.27f, 0.17f), 0.2f}},
    {"chrome", {glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.40f, 0.40f, 0.40f), glm::vec3(0.77f, 0.77f, 0.77f), 0.6f}},
    {"copper", {glm::vec3(0.19f, 0.07f, 0.02f), glm::vec3(0.70f, 0.27f, 0.08f), glm::vec3(0.26f, 0.14f, 0.09f), 0.1f}}
};
MaterialTest::MaterialTest()
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
	{//物体
        objects.push_back(make_shared<Object>());
        objects[0]->shader = make_shared<Shader>("res/shader/2Light/3Material/Default.shader");
        objects[0]->vao = make_shared<VertexArray>();
        objects[0]->vbo = make_shared<VertexBuffer>(vertices,sizeof(vertices));
        objects[0]->layout = make_shared<VertexBufferLayout>();
        objects[0]->layout->Push<float>(3);//pos
        objects[0]->layout->Push<float>(3);//nor
        objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
	}
    {//光源
        objects.push_back(make_shared<Object>());
        objects[1]->shader = make_shared<Shader>("res/shader/2Light/3Material/light.shader");
        objects[1]->pos = { 1.2f, 1.0f, 2.0f };
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

        objects[2]->shader = make_shared<Shader>("res/shader/axis.shader");
        objects[2]->shader->Bind();
        objects[2]->shader->SetUniformMat4f("model", glm::mat4(1));//默认静止，所以模型矩阵直接写死，不在渲染循环更新

        objects[2]->vao = make_shared<VertexArray>();
        objects[2]->vbo = make_shared<VertexBuffer>(axisVectices, sizeof(axisVectices));
        objects[2]->layout = make_shared<VertexBufferLayout>();
        objects[2]->layout->Push<float>(3);//位置
        objects[2]->vao->AddBuffer(*objects[2]->vbo, *objects[2]->layout);
    }
}

void MaterialTest::OnRender()
{
    GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    objects[0]->shader->Bind();
    objects[0]->vao->Bind();
    objects[0]->shader->SetUniformMat4f("model", glm::mat4(1));
    objects[0]->shader->SetUniformMat4f("view", m_View);
    objects[0]->shader->SetUniformMat4f("projection", m_Proj);
    objects[0]->shader->SetUniform3f("material.ambient", MaterialsTable[MatName].ambient);
    objects[0]->shader->SetUniform3f("material.diffuse", MaterialsTable[MatName].diffuse);
    objects[0]->shader->SetUniform3f("material.specular", MaterialsTable[MatName].specular);
    objects[0]->shader->SetUniform1f("material.shininess",128* MaterialsTable[MatName].shininess);
    objects[0]->shader->SetUniform3f("light.position", objects[1]->pos);
    objects[0]->shader->SetUniform3f("light.ambient", LightColor);
    objects[0]->shader->SetUniform3f("light.diffuse", LightColor);
    objects[0]->shader->SetUniform3f("light.specular", LightColor);
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
    objects[1]->shader->SetUniform3f("LightColor", LightColor);
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

void MaterialTest::OnImGuiRender()
{
    ImGui::Text("Mat Pan");
    int i = 1;
    for (auto& it : MaterialsTable)
    {
        auto color = ImVec4(it.second.diffuse[0], it.second.diffuse[1], it.second.diffuse[2], 1);
        if (ImGui::ColorButton(it.first.c_str(), color, 0, ImVec2(30, 30)))
            MatName = it.first;
        if(i%4!=0)
            ImGui::SameLine();
        i++;
    }
    ImGui::NewLine();
    ImGui::ColorEdit3("LightColor", &LightColor.x, ImGuiColorEditFlags_Float| ImGuiColorEditFlags_NoInputs);
}
