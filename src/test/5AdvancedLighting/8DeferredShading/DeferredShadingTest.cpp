#include "DeferredShadingTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include "Model.h"
#include "imgui/imgui.h"
DeferredShadingTest::DeferredShadingTest()
{
	//------------配置渲染对象-----------//
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
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(QuadVertices, sizeof(QuadVertices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(2);
		objects[0]->layout->Push<float>(2);
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
	}

	float CubeVertices[] = {
		// positions          
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
	{
		objects.push_back(make_shared<Object>());
		objects[1]->vao = make_shared<VertexArray>();
		objects[1]->vbo = make_shared<VertexBuffer>(CubeVertices, sizeof(CubeVertices));
		objects[1]->layout = make_shared<VertexBufferLayout>();
		objects[1]->layout->Push<float>(3);
		objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[1]->layout);
	}
	nanosuit = make_shared<Model>(GetResDir() + "res/model/nanosuit/nanosuit.obj");

	int length = 6, nums = 3;//3m x3m, 3 x 3个
	int offset = length / (nums - 1);
	for (int x = -length / 2; x <= length / 2; x += offset) {
		for (int z = -length / 2; z <= length / 2; z += offset) {
			ObjectPositions.push_back(glm::vec3(x, -3, z));
		}
	}

	const unsigned int NR_LIGHTS = 32;

	srand(13);
	for (unsigned int i = 0; i < NR_LIGHTS; i++)
	{
		float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0); //[-3,3]
		float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0); //[-4,2]
		float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0); //[-3,3]
		lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // [0.5,1]
		float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); //[0.5,1]
		float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); //[0.5,1]
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}

	vector<string>ShaderNames = { "Nanosuit","NanosuitTBN", "Quad","Light"};
	for (int i = 0; i < ShaderNames.size(); i++) {
		string path = GetResDir() + "res/shader/5AdvancedLighting/8DeferredShading/" + ShaderNames[i] + ".shader";
		Shaders[ShaderNames[i]] = make_shared<Shader>(path);
	}
	Shaders["Quad"]->Bind();
	Shaders["Quad"]->SetUniform1i("GPosition", 0);
	Shaders["Quad"]->SetUniform1i("GNormal", 1);
	Shaders["Quad"]->SetUniform1i("GColorSpec", 2);

	{//-----------配置GBuffer------------//
		GLCall(glGenFramebuffers(1, &GBuffer));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, GBuffer));
		// - 位置缓冲
		GLCall(glGenTextures(1, &GPosition));
		GLCall(glBindTexture(GL_TEXTURE_2D, GPosition));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GPosition, 0));
		// - 法线缓冲
		GLCall(glGenTextures(1, &GNormal));
		GLCall(glBindTexture(GL_TEXTURE_2D, GNormal));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, GNormal, 0));
		// - 颜色+镜面缓冲
		GLCall(glGenTextures(1, &GColorSpec));
		GLCall(glBindTexture(GL_TEXTURE_2D, GColorSpec));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
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

}

void DeferredShadingTest::OnRender()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, GBuffer));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
	string Nanosuit = isNormal ? "Nanosuit" : "NanosuitTBN";
	Shaders[Nanosuit]->Bind();
	Shaders[Nanosuit]->SetUniformMat4f("view", m_View);
	Shaders[Nanosuit]->SetUniformMat4f("projection", m_Proj);

	for (int i = 0; i < ObjectPositions.size(); i++) {
		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, ObjectPositions[i]);
		model = glm::scale(model, glm::vec3(0.25));
		Shaders[Nanosuit]->SetUniformMat4f("model", model);
		nanosuit->DrawModel(Shaders[Nanosuit]);
	}
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));

	objects[0]->vao->Bind();
	Shaders["Quad"]->Bind();
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, GPosition));
	GLCall(glActiveTexture(GL_TEXTURE0+1));
	GLCall(glBindTexture(GL_TEXTURE_2D, GNormal));
	GLCall(glActiveTexture(GL_TEXTURE0+2));
	GLCall(glBindTexture(GL_TEXTURE_2D, GColorSpec));
	Shaders["Quad"]->SetUniform3f("ViewPos", Camera::GetInstance()->Position);
	for (int i = 0; i < lightColors.size(); i++) {
		string LightStr = "Lights[" + to_string(i) + "].";
		string Color = "Lights[" + to_string(i) + "].Color";

		Shaders["Quad"]->SetUniform3f(LightStr+"Position", lightPositions[i]);
		Shaders["Quad"]->SetUniform3f(LightStr+"Color", 2.f*lightColors[i]);
		const float linear = 0.7f; 
		const float quadratic = 1.8f;
		Shaders["Quad"]->SetUniform1f(LightStr + "Linear", linear);
		Shaders["Quad"]->SetUniform1f(LightStr + "Quadratic", quadratic);
	}
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, GBuffer));
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0)); // 写入到默认帧缓冲
	GLCall(glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	objects[1]->vao->Bind();
	Shaders["Light"]->Bind();
	Shaders["Light"]->SetUniformMat4f("view", m_View);
	Shaders["Light"]->SetUniformMat4f("projection", m_Proj);
	for (int i = 0; i < lightColors.size(); i++) {
		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, lightPositions[i]);
		model = glm::scale(model, glm::vec3(0.25));
		Shaders["Light"]->SetUniformMat4f("model", model);
		Shaders["Light"]->SetUniform3f("LightColor", lightColors[i]);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}
	//光体积不想搞了
}

void DeferredShadingTest::OnImGuiRender()
{
	ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);

	if (ImGui::Button("Normal"))
		isNormal = !isNormal;
	string Normal = isNormal ? "isON" : "isOFF";
	ImGui::SameLine(100);
	ImGui::Text( Normal.c_str());
}
