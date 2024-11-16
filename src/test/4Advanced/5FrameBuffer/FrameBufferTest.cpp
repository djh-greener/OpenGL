#include "FrameBufferTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "imgui/imgui.h"
FrameBufferTest::FrameBufferTest()
{
	float cubeVertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		// Right face
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		// Top face
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
	};
	float quadVertices[] = {
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};
	fbo = make_shared<FrameBuffer>();
	fbo->AddAttachment(AttachmentType::Default);
	{//正方体
		objects.push_back(make_shared<Object>());
		objects[0]->shader = make_shared<Shader>("res/shader/4Advanced/5FrameBuffer/Cube.shader");
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(2);
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
		objects[0]->texs.push_back(make_shared<Texture>("res/image/container.jpg"));//正方体的纹理
	}
	{//四边形
		objects.push_back(make_shared<Object>());
		objects[1]->vao = make_shared<VertexArray>();
		objects[1]->vbo = make_shared<VertexBuffer>(quadVertices, sizeof(quadVertices));
		objects[1]->layout = make_shared<VertexBufferLayout>();
		objects[1]->layout->Push<float>(2);
		objects[1]->layout->Push<float>(2);
		objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[1]->layout);
	}
	shaders["Quad"] = make_shared<Shader>("res/shader/4Advanced/5FrameBuffer/Quad.shader");
	shaders["Reverse"] = make_shared<Shader>("res/shader/4Advanced/5FrameBuffer/Reverse.shader");
	shaders["GrayScale"] = make_shared<Shader>("res/shader/4Advanced/5FrameBuffer/GrayScale.shader"); 
	shaders["SharpenKernel"] = make_shared<Shader>("res/shader/4Advanced/5FrameBuffer/SharpenKernel.shader");
	shaders["BlurKernel"] = make_shared<Shader>("res/shader/4Advanced/5FrameBuffer/BlurKernel.shader"); 
	shaders["EdgeDetectionKernel"] = make_shared<Shader>("res/shader/4Advanced/5FrameBuffer/EdgeDetectionKernel.shader");
}

void FrameBufferTest::OnRender()
{
	// pass1 渲染场景到纹理
	fbo->Bind();
	
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	objects[0]->shader->Bind();
	objects[0]->vao->Bind();
	objects[0]->texs[0]->Bind(0);
	objects[0]->shader->SetUniformMat4f("projection", m_Proj);
	objects[0]->shader->SetUniformMat4f("view", m_View);
	objects[0]->shader->SetUniform1i("diffuse", 0);
	objects[0]->shader->SetUniformMat4f("model", glm::translate(glm::mat4(1), glm::vec3(2, 0.5 + 0.06, 2)));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	objects[0]->shader->SetUniformMat4f("model", glm::translate(glm::mat4(1), glm::vec3(-1, 0.5 + 0.06, -1)));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	fbo->CompleteCheck();

	// pass2 显示纹理
	fbo->UnBind();
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
	glDisable(GL_DEPTH_TEST);
	fbo->ColorBuffer->Bind(1);//
	shaders[CurrentShader]->Bind();
	objects[1]->vao->Bind();
	shaders[CurrentShader]->SetUniform1i("screenTexture", 1);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}

void FrameBufferTest::OnImGuiRender()
{
	if (ImGui::Button("Quad"))CurrentShader = "Quad";
	if (ImGui::Button("Reverse"))CurrentShader = "Reverse";
	if (ImGui::Button("GrayScale"))CurrentShader = "GrayScale";
	if (ImGui::Button("SharpenKernel"))CurrentShader = "SharpenKernel";
	if (ImGui::Button("BlurKernel"))CurrentShader = "BlurKernel"; 
	if (ImGui::Button("EdgeDetectionKernel"))CurrentShader = "EdgeDetectionKernel"; 


}
