#include "SSAOTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include "Model.h"
#include"Mesh.h"
#include "imgui/imgui.h"
#include <random>
SSAOTest::SSAOTest()
{
	//------------配置渲染对象-----------//
	{
		float QuadVertices[] = {
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};
		{//铺屏四边形
			objects.push_back(make_shared<Object>());
			objects[0]->vao = make_shared<VertexArray>();
			objects[0]->vbo = make_shared<VertexBuffer>(QuadVertices, sizeof(QuadVertices));
			objects[0]->layout = make_shared<VertexBufferLayout>();
			objects[0]->layout->Push<float>(2);
			objects[0]->layout->Push<float>(2);
			objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
		}
		float planeVertices[] = {
			// positions            // normals         // texcoords
			 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

			 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
			 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
		};
		{//地板
			objects.push_back(make_shared<Object>());
			objects[1]->vao = make_shared<VertexArray>();
			objects[1]->vbo = make_shared<VertexBuffer>(planeVertices, sizeof(planeVertices));
			objects[1]->layout = make_shared<VertexBufferLayout>();
			objects[1]->layout->Push<float>(3);
			objects[1]->layout->Push<float>(3);
			objects[1]->layout->Push<float>(2);
			objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[1]->layout);
		}
		float cubeVertices[] = {
			// Positions       
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
			-0.5f,  0.5f, -0.5f
		};
		{//光源
			objects.push_back(make_shared<Object>());
			objects[2]->shader = make_shared<Shader>(GetResDir() + "res/shader/light.shader");
			objects[2]->vao = make_shared<VertexArray>();
			objects[2]->vbo = make_shared<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
			objects[2]->layout = make_shared<VertexBufferLayout>();
			objects[2]->layout->Push<float>(3);
			objects[2]->vao->AddBuffer(*objects[2]->vbo, *objects[2]->layout);
			objects[2]->pos = glm::vec3(0, 1, 0);
		}
		nanosuit = make_shared<Model>(GetResDir() + "res/model/nanosuit/nanosuit.obj");

	}


	//------------配置GBuffer-----------//
	{
		GLCall(glGenFramebuffers(1, &GBuffer));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, GBuffer));
		// - 位置缓冲
		GLCall(glGenTextures(1, &GPosition));
		GLCall(glBindTexture(GL_TEXTURE_2D, GPosition));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GPosition, 0));
		// - 法线缓冲
		GLCall(glGenTextures(1, &GNormal));
		GLCall(glBindTexture(GL_TEXTURE_2D, GNormal));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, GNormal, 0));
		// - 颜色+镜面缓冲
		GLCall(glGenTextures(1, &GColorSpec));
		GLCall(glBindTexture(GL_TEXTURE_2D, GColorSpec));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, GColorSpec, 0));

		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		GLCall(glDrawBuffers(3, attachments));

		unsigned int RBO;
		GLCall(glGenRenderbuffers(1, &RBO));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, RBO));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO));

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	//------------配置采样核心----------//
	{
		//生成采样核心半球
		uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // 随机浮点数，范围0.0 - 1.0
		default_random_engine generator;
		auto lerp = [](GLfloat a, GLfloat b, GLfloat f) {return a + f * (b - a); };

		for (GLuint i = 0; i < 64; ++i)
		{
			glm::vec3 sample(
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator)
			);
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			GLfloat scale = GLfloat(i) / 64.0;
			scale = lerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			ssaoKernel.push_back(sample);
		}
		//生成随机旋转向量,定义在TBN空间
		
		for (GLuint i = 0; i < 16; i++)
		{
			glm::vec3 noise(
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator) * 2.0 - 1.0,
				0.0f);
			ssaoNoise.push_back(noise);
		}
		GLCall(glGenTextures(1, &noiseTexture));
		GLCall(glBindTexture(GL_TEXTURE_2D, noiseTexture));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	}

	//------------SSAO Buffer----------//
	{
		GLCall(glGenFramebuffers(1, &SSAOFBO));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, SSAOFBO));
		GLCall(glGenTextures(1, &SSAOColorBuffer));
		
		GLCall(glBindTexture(GL_TEXTURE_2D, SSAOColorBuffer));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SSAOColorBuffer, 0));

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	}

	//---------着色器加载和初始化-------//
	{
		vector<string>ShaderNames = { "Plane", "NanosuitTBN",  "SSAOQuad","Quad", };
		for (int i = 0; i < ShaderNames.size(); i++) {
			string path = GetResDir() + "res/shader/5AdvancedLighting/9SSAO/" + ShaderNames[i] + ".shader";
			Shaders[ShaderNames[i]] = make_shared<Shader>(path);
		}

		Shaders["SSAOQuad"]->Bind();
		Shaders["SSAOQuad"]->SetUniform1i("GPosition", 0);
		Shaders["SSAOQuad"]->SetUniform1i("GNormal", 1);
		Shaders["SSAOQuad"]->SetUniform1i("TexNoise", 2);
		for (int i = 0; i < 64; i++) {
			Shaders["SSAOQuad"]->SetUniform3f("samples[" + to_string(i) + "]", ssaoKernel[i]);
		}
		Shaders["Quad"]->Bind();
		Shaders["Quad"]->SetUniform1i("GPosition", 0);
		Shaders["Quad"]->SetUniform1i("GNormal", 1);
		Shaders["Quad"]->SetUniform1i("GColorSpec", 2);
		Shaders["Quad"]->SetUniform1i("Occlusion", 3);
	}
}

void SSAOTest::OnRender()
{
	//Pass 1  Get GBuffer
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, GBuffer));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));

		glm::mat4 model = glm::mat4(1);
		Shaders["Plane"]->Bind();
		objects[1]->vao->Bind();
		Shaders["Plane"]->SetUniformMat4f("model", model);
		Shaders["Plane"]->SetUniformMat4f("view", m_View);
		Shaders["Plane"]->SetUniformMat4f("projection", m_Proj);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		
		Shaders["NanosuitTBN"]->Bind();
		model = glm::translate(model, nanosuit_pos);
		model = glm::rotate(model, glm::radians(-90.f),glm::vec3(1,0,0));
		Shaders["NanosuitTBN"]->SetUniformMat4f("model", model);
		Shaders["NanosuitTBN"]->SetUniformMat4f("view", m_View);
		Shaders["NanosuitTBN"]->SetUniformMat4f("projection", m_Proj);
		nanosuit->DrawModel(Shaders["NanosuitTBN"]);
	//GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	//Pass 2 Render SSAO 
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, SSAOFBO));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

		Shaders["SSAOQuad"]->Bind();
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, GPosition));
		GLCall(glActiveTexture(GL_TEXTURE0 + 1));
		GLCall(glBindTexture(GL_TEXTURE_2D, GNormal));
		GLCall(glActiveTexture(GL_TEXTURE0 + 2));
		GLCall(glBindTexture(GL_TEXTURE_2D, noiseTexture));

		Shaders["SSAOQuad"]->SetUniformMat4f("projection", m_Proj);
		Shaders["SSAOQuad"]->SetUniform1i("SCR_WIDTH", SCR_WIDTH);
		Shaders["SSAOQuad"]->SetUniform1i("SCR_HEIGHT", SCR_HEIGHT);
		objects[0]->vao->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

	//Pass 3 : Draw Sence
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		
		objects[0]->vao->Bind();
		Shaders["Quad"]->Bind();
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, GPosition));
		GLCall(glActiveTexture(GL_TEXTURE0 + 1));
		GLCall(glBindTexture(GL_TEXTURE_2D, GNormal));
		GLCall(glActiveTexture(GL_TEXTURE0 + 2));
		GLCall(glBindTexture(GL_TEXTURE_2D, GColorSpec));
		GLCall(glActiveTexture(GL_TEXTURE0 + 3));
		GLCall(glBindTexture(GL_TEXTURE_2D, SSAOColorBuffer));
		Shaders["Quad"]->SetUniform3f("ViewPos", Camera::GetInstance()->Position);
		glm::vec3 LightDirView = glm::mat3(m_View) * -objects[2]->pos;//转换到视图空间
		Shaders["Quad"]->SetUniform3f("Light.Direction", LightDirView);
		Shaders["Quad"]->SetUniform3f("Light.Color", glm::vec3(0.2,0.2,0.7));
		Shaders["Quad"]->SetUniform1i("Method", DrawMethod);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

}

void SSAOTest::OnImGuiRender()
{
	ImGui::SliderFloat3("NanosuitPos", &nanosuit_pos.x, -10, 10);
	if (ImGui::Button("Default")) {
		DrawMethod = 0;
	}
	if (ImGui::Button("Position")) {
		DrawMethod = 1;
	}
	if (ImGui::Button("Normal")) {
		DrawMethod = 2;
	}
	if (ImGui::Button("Color")) {
		DrawMethod = 3;
	}
	if (ImGui::Button("SSAO")) {
		DrawMethod = 4;
	}
}
