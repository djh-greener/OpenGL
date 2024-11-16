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
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // 左上
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
    };
    {//Default
        objects.push_back(make_shared<Object>());
        objects[0]->shader = make_shared<Shader>("res/shader/1GetStart/3Texture/TextureTest.shader");
        objects[0]->shader->Bind();
        objects[0]->shader->SetUniform1i("Texture0", 0);//设置着色器里的采样器对应的纹理单元
        objects[0]->shader->SetUniform1i("Texture1", 1);
        objects[0]->vao = make_shared<VertexArray>();
        objects[0]->vbo = make_shared<VertexBuffer>(vertices, sizeof(vertices));
        objects[0]->layout = make_shared<VertexBufferLayout>();
        objects[0]->layout->Push<float>(3);//位置
        objects[0]->layout->Push<float>(3);//颜色
        objects[0]->layout->Push<float>(2);//纹理坐标
        objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
        objects[0]->texs.push_back(make_shared<Texture>("res/image/container.jpg"));
        objects[0]->texs.push_back(make_shared<Texture>("res/image/awesomeface.png"));
        //将纹理对象绑定到对应纹理单元
        for (int i = 0; i < objects[0]->texs.size(); i++)
            objects[0]->texs[i]->Bind(i);
    }
    {//First:修改片段着色器，仅让笑脸图案朝另一个方向看
        objects.push_back(make_shared<Object>());
        objects[1]->shader = make_shared<Shader>("res/shader/1GetStart/3Texture/TextureFirstTest.shader");
        objects[1]->shader->Bind();
        objects[1]->shader->SetUniform1i("Texture0", 0);//设置着色器里的采样器对应的纹理单元
        objects[1]->shader->SetUniform1i("Texture1", 1);
    }
    {//Second:尝试用不同的纹理环绕方式，设定一个从0.0f到2.0f范围内的（而不是原来的0.0f到1.0f）纹理坐标。
        //试试看能不能在箱子的角落放置4个笑脸 记得一定要试试其它的环绕方式。
        float tmp[] = {
            //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // 右上
                 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // 右下
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f,    // 左上
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // 右上
        };
        objects.push_back(make_shared<Object>());
        objects[2]->vao = make_shared<VertexArray>();
        objects[2]->vbo = make_shared<VertexBuffer>(tmp, sizeof(tmp));
        objects[2]->vao->AddBuffer(*objects[2]->vbo, *objects[0]->layout);
    }
    {//Third:尝试在矩形上只显示纹理图像的中间一部分，修改纹理坐标，达到能看见单个的像素的效果。
        //尝试使用GL_NEAREST的纹理过滤方式让像素显示得更清晰
        float tmp[] = {
            //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   .75f, .75f,   // 右上
                 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   .75f, 0.25f,   // 右下
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.25f, 0.25f,   // 左下
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.25f, 0.25f,   // 左下
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.25f, .75f,    // 左上
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   .75f, .75f,   // 右上
        };
        objects.push_back(make_shared<Object>());
        objects[3]->vao = make_shared<VertexArray>();
        objects[3]->vbo = make_shared<VertexBuffer>(tmp, sizeof(tmp));
        objects[3]->vao->AddBuffer(*objects[3]->vbo, *objects[0]->layout);
    }
    {//Fourth:使用一个uniform变量作为mix函数的第三个参数来改变两个纹理可见度，使用上和下键来改变箱子或笑脸的可见度
        objects.push_back(make_shared<Object>());
        objects[4]->shader = make_shared<Shader>("res/shader/1GetStart/3Texture/TextureFourthTest.shader");
        objects[4]->shader->Bind();
        objects[4]->shader->SetUniform1i("Texture0", 0);//设置着色器里的采样器对应的纹理单元
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
