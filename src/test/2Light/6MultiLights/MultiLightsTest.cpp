#include "MultiLightsTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"

#include "imgui/imgui.h"
MultiLightsTest::MultiLightsTest()
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
    lightColor = glm::vec3(1);
    dirLightdir = glm::vec3(-0.2f, -1.0f, -0.3f);
    
    {//物体
        objects.push_back(make_shared<Object>());
        objects[0]->shader = make_shared<Shader>(GetResDir() + "res/shader/2Light/6MultiLights/Default.shader");
        objects[0]->vao = make_shared<VertexArray>();
        objects[0]->vbo = make_shared<VertexBuffer>(vertices, sizeof(vertices));
        objects[0]->layout = make_shared<VertexBufferLayout>();
        objects[0]->layout->Push<float>(3);
        objects[0]->layout->Push<float>(3);
        objects[0]->layout->Push<float>(2);
        objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
        objects[0]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/container2.png"));
        objects[0]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/container2_specular.png"));

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

void MultiLightsTest::OnRender()
{

    GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    objects[0]->shader->Bind();
    objects[0]->vao->Bind();
    objects[0]->shader->SetUniformMat4f("view", m_View);
    objects[0]->shader->SetUniformMat4f("projection", m_Proj);
    objects[0]->shader->SetUniform1i("material.diffuse", 0);
    objects[0]->shader->SetUniform1i("material.specular", 1);
    objects[0]->shader->SetUniform1f("material.shininess", 32);
    objects[0]->shader->SetUniform3f("viewPos", Camera::GetInstance()->Position);
    //定向光
    objects[0]->shader->SetUniform3f("dirLight.direction", dirLightdir);
    objects[0]->shader->SetUniform3f("dirLight.ambient", 0.05f*lightColor);
    objects[0]->shader->SetUniform3f("dirLight.diffuse", 0.8f*lightColor);
    objects[0]->shader->SetUniform3f("dirLight.specular", lightColor);

    //点光源
    for (int i = 0; i < 4; i++)
    {
        string IdxStr = "pointLights[" + to_string(i) + "].";
        objects[0]->shader->SetUniform3f(IdxStr + "position", pointLightPositions[i]);
        objects[0]->shader->SetUniform1f(IdxStr + "constant", 1.0f);
        objects[0]->shader->SetUniform1f(IdxStr + "linear", 0.09f);
        objects[0]->shader->SetUniform1f(IdxStr + "quadratic", 0.032f);
        objects[0]->shader->SetUniform3f(IdxStr + "ambient", 0.05f * lightColor);
        objects[0]->shader->SetUniform3f(IdxStr + "diffuse", 0.8f * lightColor);
        objects[0]->shader->SetUniform3f(IdxStr + "specular", lightColor);
    }
    //聚光
    objects[0]->shader->SetUniform3f("spotLight.position", Camera::GetInstance()->Position);
    objects[0]->shader->SetUniform3f("spotLight.direction", Camera::GetInstance()->Front);
    objects[0]->shader->SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    objects[0]->shader->SetUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(17.5)));
    objects[0]->shader->SetUniform1f("spotLight.constant", 1.0f);
    objects[0]->shader->SetUniform1f("spotLight.linear", 0.09f);
    objects[0]->shader->SetUniform1f("spotLight.quadratic", 0.032f);
    objects[0]->shader->SetUniform3f("spotLight.ambient", 0.05f * lightColor);
    objects[0]->shader->SetUniform3f("spotLight.diffuse", 0.8f * lightColor);
    objects[0]->shader->SetUniform3f("spotLight.specular", lightColor);
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
    for (unsigned int i = 0; i < 10; i++)
    {
        auto model = glm::translate(glm::mat4(1), cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        objects[0]->shader->SetUniformMat4f("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    for (unsigned int i = 0; i < 4; i++)
    {
        objects[1]->shader->Bind();
        objects[0]->vao->Bind();
        auto model = glm::translate(glm::mat4(1.f), pointLightPositions[i]);
        model = glm::scale(model, 0.2f * glm::vec3(1));
        objects[1]->shader->SetUniformMat4f("model", model);
        objects[1]->shader->SetUniformMat4f("view", m_View);
        objects[1]->shader->SetUniformMat4f("projection", m_Proj);
        objects[1]->shader->SetUniform3f("LightColor", lightColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    

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

void MultiLightsTest::OnImGuiRender()
{
    ImGui::ColorEdit3("LightColor", &lightColor.x, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs);
    ImGui::DragFloat3("DirLightDir", &dirLightdir.x);
    ImGui::SliderFloat3("PointLightPos_1", &pointLightPositions[0].x, -10, 10);
    ImGui::SliderFloat3("PointLightPos_2", &pointLightPositions[1].x, -10, 10);
    ImGui::SliderFloat3("PointLightPos_3", &pointLightPositions[2].x, -10, 10);
    ImGui::SliderFloat3("PointLightPos_4", &pointLightPositions[3].x, -10, 10);

}
