#include "ColorTest.h"

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

ColorTest::ColorTest()
{
    float vertices[] = {
 -0.5f, -0.5f, -0.5f, 
  0.5f, -0.5f, -0.5f, 
  0.5f,  0.5f, -0.5f,  
  0.5f,  0.5f, -0.5f,  
 -0.5f,  0.5f, -0.5f, 
 -0.5f, -0.5f, -0.5f, 

 -0.5f, -0.5f,  0.5f, 
  0.5f, -0.5f,  0.5f,  
  0.5f,  0.5f,  0.5f,  
  0.5f,  0.5f,  0.5f,  
 -0.5f,  0.5f,  0.5f,  
 -0.5f, -0.5f,  0.5f, 

 -0.5f,  0.5f,  0.5f,  
 -0.5f,  0.5f, -0.5f, 
 -0.5f, -0.5f, -0.5f, 
 -0.5f, -0.5f, -0.5f, 
 -0.5f, -0.5f,  0.5f, 
 -0.5f,  0.5f,  0.5f,  

  0.5f,  0.5f,  0.5f,  
  0.5f,  0.5f, -0.5f,  
  0.5f, -0.5f, -0.5f, 
  0.5f, -0.5f, -0.5f, 
  0.5f, -0.5f,  0.5f,  
  0.5f,  0.5f,  0.5f,  

 -0.5f, -0.5f, -0.5f, 
  0.5f, -0.5f, -0.5f, 
  0.5f, -0.5f,  0.5f,  
  0.5f, -0.5f,  0.5f,  
 -0.5f, -0.5f,  0.5f, 
 -0.5f, -0.5f, -0.5f, 

 -0.5f,  0.5f, -0.5f, 
  0.5f,  0.5f, -0.5f,  
  0.5f,  0.5f,  0.5f,  
  0.5f,  0.5f,  0.5f,  
 -0.5f,  0.5f,  0.5f,  
 -0.5f,  0.5f, -0.5f, 
    };
    {//Box
        objects.push_back(make_shared<Object>());
        objects[0]->shader = make_shared<Shader>("res/shader/2Light/1Color/ColorTest.shader");
        objects[0]->shader->Bind();
        objects[0]->shader->SetUniformMat4f("model", glm::mat4(1.f));

        objects[0]->vao = make_shared<VertexArray>();
        objects[0]->vbo = make_shared<VertexBuffer>(vertices, sizeof(vertices));
        objects[0]->layout = make_shared<VertexBufferLayout>();
        objects[0]->layout->Push<float>(3);
        objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
    }
    {//Light
        objects.push_back(make_shared<Object>());
        objects[1]->vao = make_shared<VertexArray>();
        objects[1]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
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

        objects[2]->shader = make_shared<Shader>("res/shader/axis.shader");
        objects[2]->shader->Bind();
        objects[2]->shader->SetUniformMat4f("model", glm::mat4(1));

        objects[2]->vao = make_shared<VertexArray>();
        objects[2]->vbo = make_shared<VertexBuffer>(axisVectices, sizeof(axisVectices));
        objects[2]->layout = make_shared<VertexBufferLayout>();
        objects[2]->layout->Push<float>(3);//Î»ÖÃ
        objects[2]->vao->AddBuffer(*objects[2]->vbo, *objects[2]->layout);
    }
}

void ColorTest::OnRender()
{
    GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    objects[0]->shader->Bind();
    objects[0]->vao->Bind();
    objects[0]->shader->SetUniform3f("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    objects[0]->shader->SetUniform3f("lightColor", glm::vec3(1.0f));
    objects[0]->shader->SetUniformMat4f("model", glm::mat4(1.f));
    objects[0]->shader->SetUniformMat4f("view", m_View);
    objects[0]->shader->SetUniformMat4f("projection", m_Proj);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    objects[0]->shader->Bind();
    objects[1]->vao->Bind();
    objects[0]->shader->SetUniform3f("objectColor", glm::vec3(1.0f));
    objects[0]->shader->SetUniform3f("lightColor", glm::vec3(1.0f));
    auto model = glm::translate(glm::mat4(1.f), objects[1]->pos);
    model = glm::scale(model, glm::vec3(0.2f));
    objects[0]->shader->SetUniformMat4f("model", model);
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

void ColorTest::OnImGuiRender()
{
    ImGui::SliderFloat3("lightPos", &objects[1]->pos.x, -5, 5);
}
