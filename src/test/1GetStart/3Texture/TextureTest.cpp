#include "TextureTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Object.h"
#include "MyDebug.h"
#include "imgui/imgui.h"
#include "Texture.h"
TextureTest::TextureTest()
{
    float vertices[] = {
        //     ---- λ�� ----       ---- ��ɫ ----     - �������� -
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // ����
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
    };
    {//Default
        objects.push_back(make_shared<Object>());
        objects[0]->shader = make_shared<Shader>("res/shader/1GetStart/3Texture/TextureTest.shader");
        objects[0]->shader->Bind();
        objects[0]->shader->SetUniform1i("Texture0", 0);//������ɫ����Ĳ�������Ӧ������Ԫ
        objects[0]->shader->SetUniform1i("Texture1", 1);
        objects[0]->vao = make_shared<VertexArray>();
        objects[0]->vbo = make_shared<VertexBuffer>(vertices, sizeof(vertices));
        objects[0]->layout = make_shared<VertexBufferLayout>();
        objects[0]->layout->Push<float>(3);//λ��
        objects[0]->layout->Push<float>(3);//��ɫ
        objects[0]->layout->Push<float>(2);//��������
        objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
        objects[0]->texs.push_back(make_shared<Texture>("res/image/container.jpg"));
        objects[0]->texs.push_back(make_shared<Texture>("res/image/awesomeface.png"));
        //���������󶨵���Ӧ����Ԫ
        for (int i = 0; i < objects[0]->texs.size(); i++)
            objects[0]->texs[i]->Bind(i);
    }
    {//First:�޸�Ƭ����ɫ��������Ц��ͼ������һ������
        objects.push_back(make_shared<Object>());
        objects[1]->shader = make_shared<Shader>("res/shader/1GetStart/3Texture/TextureFirstTest.shader");
        objects[1]->shader->Bind();
        objects[1]->shader->SetUniform1i("Texture0", 0);//������ɫ����Ĳ�������Ӧ������Ԫ
        objects[1]->shader->SetUniform1i("Texture1", 1);
    }
    {//Second:�����ò�ͬ�������Ʒ�ʽ���趨һ����0.0f��2.0f��Χ�ڵģ�������ԭ����0.0f��1.0f���������ꡣ
        //���Կ��ܲ��������ӵĽ������4��Ц�� �ǵ�һ��Ҫ���������Ļ��Ʒ�ʽ��
        float tmp[] = {
            //     ---- λ�� ----       ---- ��ɫ ----     - �������� -
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // ����
                 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // ����
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f,    // ����
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // ����
        };
        objects.push_back(make_shared<Object>());
        objects[2]->vao = make_shared<VertexArray>();
        objects[2]->vbo = make_shared<VertexBuffer>(tmp, sizeof(tmp));
        objects[2]->vao->AddBuffer(*objects[2]->vbo, *objects[0]->layout);
    }
    {//Third:�����ھ�����ֻ��ʾ����ͼ����м�һ���֣��޸��������꣬�ﵽ�ܿ������������ص�Ч����
        //����ʹ��GL_NEAREST��������˷�ʽ��������ʾ�ø�����
        float tmp[] = {
            //     ---- λ�� ----       ---- ��ɫ ----     - �������� -
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   .75f, .75f,   // ����
                 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   .75f, 0.25f,   // ����
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.25f, 0.25f,   // ����
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.25f, 0.25f,   // ����
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.25f, .75f,    // ����
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   .75f, .75f,   // ����
        };
        objects.push_back(make_shared<Object>());
        objects[3]->vao = make_shared<VertexArray>();
        objects[3]->vbo = make_shared<VertexBuffer>(tmp, sizeof(tmp));
        objects[3]->vao->AddBuffer(*objects[3]->vbo, *objects[0]->layout);
    }
    {//Fourth:ʹ��һ��uniform������Ϊmix�����ĵ������������ı���������ɼ��ȣ�ʹ���Ϻ��¼����ı����ӻ�Ц���Ŀɼ���
        objects.push_back(make_shared<Object>());
        objects[4]->shader = make_shared<Shader>("res/shader/1GetStart/3Texture/TextureFourthTest.shader");
        objects[4]->shader->Bind();
        objects[4]->shader->SetUniform1i("Texture0", 0);//������ɫ����Ĳ�������Ӧ������Ԫ
        objects[4]->shader->SetUniform1i("Texture1", 1);
    }
}

void TextureTest::OnRender()
{
    GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    switch (exercise)
    {
    case Default:
        objects[0]->shader->Bind();
        objects[0]->vao->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        break;
    case First:
        objects[1]->shader->Bind();
        objects[0]->vao->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        break;
    case Second:
        Texture::SetRound(GL_REPEAT);
        objects[0]->shader->Bind();
        objects[2]->vao->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        Texture::SetRound(0);
        break;
    case Third:
        Texture::SetFilter(GL_NEAREST);
        objects[0]->shader->Bind();
        objects[3]->vao->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        Texture::SetFilter(0);
        break;
    case Fourth:
        objects[4]->shader->Bind();
        objects[4]->shader->SetUniform1f("factor", factor);
        objects[0]->vao->Bind();
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        break;
    default:
        break;
    }
}

void TextureTest::OnImGuiRender()
{
    if (ImGui::Button("Default"))
        exercise = Default;
    if (ImGui::Button("First"))
        exercise = First;
    if (ImGui::Button("Second"))
        exercise = Second;
    if (ImGui::Button("Third"))
        exercise = Third;
    if (ImGui::Button("Fourth"))
        exercise = Fourth;
    ImGui::SameLine();
    ImGui::SliderFloat("factor", &factor, 0, 1);
}
