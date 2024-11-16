#include "HDRTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include "imgui/imgui.h"
HDRTest::HDRTest()
{
	lightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
	lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
	lightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
	lightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));
	lightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
	lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
	lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
	lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));

	float vertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};
	{
		objects.push_back(make_shared<Object>());
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(vertices, sizeof(vertices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(2);
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
		objects[0]->texs.push_back(make_shared<Texture>("res/image/wood.png", true));
	}

	float quadVertices[] = {
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f,0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,1.0f, 1.0f
	};
	{
		objects.push_back(make_shared<Object>());
		objects[1]->vao = make_shared<VertexArray>();
		objects[1]->vbo = make_shared<VertexBuffer>(quadVertices, sizeof(quadVertices));
		objects[1]->layout = make_shared<VertexBufferLayout>();
		objects[1]->layout->Push<float>(3);
		objects[1]->layout->Push<float>(2);
		objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[1]->layout);
	}
	{
		Shaders["Scene"] = make_shared<Shader>("res/shader/5AdvancedLighting/6HDR/Scene.shader");
		Shaders["Quad"] = make_shared<Shader>("res/shader/5AdvancedLighting/6HDR/Quad.shader");

	}
	GLCall(glGenTextures(1, &HDRTexture));
	GLCall(glBindTexture(GL_TEXTURE_2D, HDRTexture));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	
	glGenRenderbuffers(1, &RBODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, RBODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);

	GLCall(glGenFramebuffers(1, &FBO));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, FBO));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, HDRTexture, 0));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBODepth));
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

}

void HDRTest::OnRender()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, FBO));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));

	objects[0]->vao->Bind();
	objects[0]->texs[0]->Bind(0);
	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
	model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
	Shaders["Scene"]->Bind();
	Shaders["Scene"]->SetUniformMat4f("model", model);
	Shaders["Scene"]->SetUniformMat4f("view", m_View);
	Shaders["Scene"]->SetUniformMat4f("projection", m_Proj);
	Shaders["Scene"]->SetUniform1i("ReverseNormal", true);
	Shaders["Scene"]->SetUniform1i("Diffuse", 0);
	for (int i = 0; i < 4; i++) {
		string VarName = "Lights[" + to_string(i) + "].";
		Shaders["Scene"]->SetUniform3f(VarName + "Position", lightPositions[i]);
		Shaders["Scene"]->SetUniform3f(VarName + "Color", lightColors[i]);
	}
	Shaders["Scene"]->SetUniform3f("ViewPos",Camera::GetInstance()->Position);
	GLCall(glDrawArrays(GL_TRIANGLES,0,36));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
	objects[1]->vao->Bind();
	GLCall(glBindTexture(GL_TEXTURE_2D, HDRTexture));
	Shaders["Quad"]->Bind();
	Shaders["Quad"]->SetUniform1i("HDRTexture", 0);
	Shaders["Quad"]->SetUniform1i("Algorithm", algorithm);
	Shaders["Quad"]->SetUniform1f("exposure", exposure);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

}

void HDRTest::OnImGuiRender()
{
	if (ImGui::Button("None")) {
		algorithm = Algorithm::None;
	}
	if (ImGui::Button("Reinhard")) {
		algorithm = Algorithm::Reinhard;
	}
	if (ImGui::Button("Exposure")) {
		algorithm = Algorithm::Exposure;
	}
	ImGui::SameLine();
	ImGui::SliderFloat("exposure", &exposure, 0.1, 10);
}
