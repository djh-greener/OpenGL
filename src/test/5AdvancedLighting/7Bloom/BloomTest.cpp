#include "BloomTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include "imgui/imgui.h"
BloomTest::BloomTest()
{
	//-------------Data Setting-----------------//
	LightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
	LightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
	LightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
	LightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));

	LightColors.push_back(glm::vec3(5.0f, 5.0f, 5.0f));
	LightColors.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
	LightColors.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
	LightColors.push_back(glm::vec3(0.0f, 5.0f, 0.0f));
	float CubeVertices[] = {
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
	{
		objects.push_back(make_shared<Object>());
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(CubeVertices, sizeof(CubeVertices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(2);
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
		objects[0]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/container2.png", true));
		objects[0]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/wood.png", true));
	}
	float QuadVertices[] = {
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};
	{
		objects.push_back(make_shared<Object>());
		objects[1]->vao = make_shared<VertexArray>();
		objects[1]->vbo = make_shared<VertexBuffer>(QuadVertices, sizeof(QuadVertices));
		objects[1]->layout = make_shared<VertexBufferLayout>();
		objects[1]->layout->Push<float>(2);
		objects[1]->layout->Push<float>(2);
		objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[1]->layout);
	}
	//-------------Shaders Loading&Setting-----------------//
	vector<string>ShaderNames = { "Cube","Light","Quad","Blur","Blend"};
	for (int i = 0; i < ShaderNames.size(); i++) {
		string path = GetResDir() + "res/shader/5AdvancedLighting/7Bloom/" + ShaderNames[i] + ".shader";
		Shaders[ShaderNames[i]] = make_shared<Shader>(path);
	}
	{
		Shaders["Blur"]->Bind();
		Shaders["Blur"]->SetUniform1i("Diffuse", 0);
	}
	//-------------FBO&TextureBuffer Loading&Setting-----------------//
	//一个FBO挂两个颜色缓冲
	GLCall(glGenFramebuffers(1, &FBO));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, FBO));
	GLCall(glGenTextures(2, HDRTextures));
	for (int i = 0; i < 2; i++) {
		GLCall(glBindTexture(GL_TEXTURE_2D, HDRTextures[i]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, HDRTextures[i], 0));
	}
	unsigned int RBODepth;
	GLCall(glGenRenderbuffers(1, &RBODepth));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, RBODepth));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBODepth));

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));


	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongBuffer);
	for (GLuint i = 0; i < 2; i++)
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]));
		GLCall(glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0));
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}
}

void BloomTest::OnRender()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, FBO));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
	//  PASS 1
	//渲染地板和箱子通用设置
	objects[0]->vao->Bind();
	Shaders["Cube"]->Bind();
	Shaders["Cube"]->SetUniformMat4f("view", m_View);
	Shaders["Cube"]->SetUniformMat4f("projection", m_Proj);
	Shaders["Cube"]->SetUniform3f("ViewPos", Camera::GetInstance()->Position);
	for (int i = 0; i < LightPositions.size(); i++) {
		string LightPos = "Lights[" + to_string(i) + "].Position";
		string LightColor = "Lights[" + to_string(i) + "].Color";
		Shaders["Cube"]->SetUniform3f(LightPos, LightPositions[i]);
		Shaders["Cube"]->SetUniform3f(LightColor, LightColors[i]);
	}

	//渲染地板
	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
	model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
	Shaders["Cube"]->SetUniformMat4f("model", model);
	Shaders["Cube"]->SetUniform1i("Diffuse", 1);
	objects[0]->texs[1]->Bind(1);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	//渲染箱子
	RenderCube();

	//渲染光源
	Shaders["Light"]->Bind();
	Shaders["Light"]->SetUniformMat4f("view", m_View);
	Shaders["Light"]->SetUniformMat4f("projection", m_Proj);
	for (int i = 0; i < LightPositions.size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, LightPositions[i]);
		model = glm::scale(model, glm::vec3(0.25f));
		Shaders["Light"]->SetUniformMat4f("model", model);
		Shaders["Light"]->SetUniform3f("LightColor", LightColors[i]);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}
	//Pass 2
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	objects[1]->vao->Bind();
	Shaders["Quad"]->Bind();
	Shaders["Quad"]->SetUniform1i("Diffuse", 0);
	
	if (Texture2Show == "Default")
	{
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, HDRTextures[0]));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	}
	else if (Texture2Show == "Brightness")
	{
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, HDRTextures[1]));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	}
	else if (Texture2Show == "Bloom")
	{

		Shaders["Blur"]->Bind();
		GLboolean horizontal = true, first_iteration = true;
		GLuint amount = 10;
		for (GLuint i = 0; i < amount; i++)
		{
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]));
			Shaders["Blur"]->SetUniform1i("horizontal", horizontal);
			GLCall(glBindTexture(GL_TEXTURE_2D, first_iteration ? HDRTextures[1] : pingpongBuffer[!horizontal]));
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		Shaders["Blend"]->Bind();
		Shaders["Blend"]->SetUniform1i("Scene", 0);
		Shaders["Blend"]->SetUniform1i("BloomBlur", 1);
		Shaders["Blend"]->SetUniform1f("exposure", 1);

		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, HDRTextures[0]));
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	}
	
}

void BloomTest::OnImGuiRender()
{
	if (ImGui::Button("Default")) {
		Texture2Show = "Default";
	}
	if (ImGui::Button("Brightness")) {
		Texture2Show = "Brightness";
	}
	if (ImGui::Button("Bloom")) {
		Texture2Show = "Bloom";
	}
}

void BloomTest::RenderCube() {
	glm::mat4 model = glm::mat4(1);

	objects[0]->texs[0]->Bind(0);
	Shaders["Cube"]->SetUniform1i("Diffuse", 0);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	Shaders["Cube"]->SetUniformMat4f("model", model);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	Shaders["Cube"]->SetUniformMat4f("model", model);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	Shaders["Cube"]->SetUniformMat4f("model", model);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
	model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(1.25));
	Shaders["Cube"]->SetUniformMat4f("model", model);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
	model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	Shaders["Cube"]->SetUniformMat4f("model", model);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	Shaders["Cube"]->SetUniformMat4f("model", model);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}
