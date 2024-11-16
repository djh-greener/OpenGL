#include "ShadowMappingTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include"FrameBuffer.h"
#include "imgui/imgui.h"
extern unsigned int SCR_WIDTH ;
extern unsigned int SCR_HEIGHT;
void GetPixel();
ShadowMappingTest::ShadowMappingTest()
{
	GLCall(glGenTextures(1, &DepthTexture));
	GLCall(glBindTexture(GL_TEXTURE_2D, DepthTexture));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	
	
	GLCall(glGenFramebuffers(1, &DepthFBO));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, DepthFBO));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,DepthTexture,0));
	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glReadBuffer(GL_NONE));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	vector<string>shadernames = { "Depth" ,"Quad","Scene"};
	string directory = "res/shader/5AdvancedLighting/3Shadows/1ShadowMapping/";
	for (auto name : shadernames) {
		Shaders[name] = make_shared<Shader>(directory + name + ".shader");
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
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(planeVertices, sizeof(planeVertices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(2);
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
		objects[0]->texs.push_back(make_shared<Texture>("res/image/wood.png", true));
	}
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
	{//立方体
		objects.push_back(make_shared<Object>());
		//立方体和地板用同一个Shader和VertexBufferLayout
		objects[1]->vao = make_shared<VertexArray>();
		objects[1]->vbo = make_shared<VertexBuffer>(CubeVertices, sizeof(CubeVertices));
		objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[0]->layout);
		objects[1]->texs.push_back(make_shared<Texture>("res/image/marble.jpg", true));
	}
	float LightVertices[] = {
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
		objects[2]->shader = make_shared<Shader>("res/shader/light.shader");
		objects[2]->vao = make_shared<VertexArray>();
		objects[2]->vbo = make_shared<VertexBuffer>(LightVertices, sizeof(LightVertices));
		objects[2]->layout = make_shared<VertexBufferLayout>();
		objects[2]->layout->Push<float>(3);
		objects[2]->vao->AddBuffer(*objects[2]->vbo, *objects[2]->layout);
		objects[2]->pos = glm::vec3(3);//定向光方向为光源位置到原点
	}
	float quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	{//深度贴图
		objects.push_back(make_shared<Object>());
		objects[3]->shader = make_shared<Shader>("res/shader/5AdvancedLighting/3Shadows/1ShadowMapping/Quad.shader");
		objects[3]->vao = make_shared<VertexArray>();
		objects[3]->vbo = make_shared<VertexBuffer>(quadVertices, sizeof(quadVertices));
		objects[3]->layout = make_shared<VertexBufferLayout>();
		objects[3]->layout->Push<float>(2);
		objects[3]->layout->Push<float>(2);
		objects[3]->vao->AddBuffer(*objects[3]->vbo, *objects[3]->layout);
	}
}

void ShadowMappingTest::OnRender()
{
	glm::mat4 Model = glm::mat4(1);
	glm::mat4 LightView = glm::mat4(1);
	glm::mat4 LightProjection = glm::mat4(1);
	LightView = glm::lookAt(objects[2]->pos, glm::vec3(0), glm::vec3(0, 1, 0));
	LightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, 1.0f, 10.0f);//定向光所以正交投影
	glm::mat4 LightSpaceMatrix = LightProjection * LightView;
	//-------渲染场景的深度贴图到纹理
	GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, DepthFBO));//先！！绑定FBO
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	Shaders["Depth"]->Bind();
	Shaders["Depth"]->SetUniformMat4f("LightSpaceMatrix", LightSpaceMatrix);
	RenderScene(Shaders["Depth"]);
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	//-------- 绘制有阴影的场景
	GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	if (DrawDepth)
	{
		GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		//GLCall(glDisable(GL_DEPTH_TEST));

		Shaders["Quad"]->Bind();
		objects[3]->vao->Bind();
		GLCall(glBindTexture(GL_TEXTURE_2D, DepthTexture));
		Shaders["Quad"]->SetUniform1i("screenTexture", 0);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		
	}
	else
	{
		//再将深度贴图渲染到屏幕
		Shaders["Scene"]->Bind();
		Shaders["Scene"]->SetUniformMat4f("view", m_View);
		Shaders["Scene"]->SetUniformMat4f("projection", m_Proj);
		Shaders["Scene"]->SetUniformMat4f("LightSpaceMatrix", LightSpaceMatrix);
		Shaders["Scene"]->SetUniform3f("LightDir", objects[2]->pos);
		Shaders["Scene"]->SetUniform3f("ViewPos", Camera::GetInstance()->Position);
		GLCall(glActiveTexture(GL_TEXTURE0 + 1));
		GLCall(glBindTexture(GL_TEXTURE_2D, DepthTexture));
		Shaders["Scene"]->SetUniform1i("ShadowMap", 1);
		RenderScene(Shaders["Scene"]);
	}

}

void ShadowMappingTest::OnImGuiRender()
{
	ImGui::SliderFloat3("LightPos", &objects[2]->pos.x,-10,10);
	if (ImGui::Button("DrawDepth"))
		DrawDepth = !DrawDepth;
	string DrawDepthStr = DrawDepth ? "ON" : "OFF";
	ImGui::SameLine(100);
	ImGui::Text("DrawDepth is %s", DrawDepthStr.c_str());
}

void GetPixel() {
	unsigned char* textureData = (unsigned char*)malloc(SCR_WIDTH * SCR_HEIGHT * 4); // 分配足够大的内存
	glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, textureData);
	// 遍历纹理数据并打印每个像素的 RGBA 值
	for (int y = 0; y < SCR_HEIGHT; y++) {
		for (int x = 0; x < SCR_WIDTH; x++) {
			int offset = (y * SCR_WIDTH + x) * 4; // 每个像素占 4 个字节
			printf("Pixel at (%d, %d): R=%d, G=%d, B=%d, A=%d\n",
				x, y,
				textureData[offset + 0],
				textureData[offset + 1],
				textureData[offset + 2],
				textureData[offset + 3]);
		}
	}
}

void ShadowMappingTest::RenderScene(shared_ptr<Shader>shader) {
	glm::mat4 Model = glm::mat4(1);

	//地板,最好不要在渲染深度贴图时渲染地板，因为一般不产生阴影
	objects[0]->vao->Bind();
	objects[0]->texs[0]->Bind(0);
	shader->SetUniformMat4f("model", glm::mat4(1));
	shader->SetUniform1i("diffuse", 0);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	//立方体
	objects[1]->vao->Bind();
	objects[1]->texs[0]->Bind(0);
	Model = glm::translate(glm::mat4(1), glm::vec3(0.0f, 1.5f, 0.0));
	Model = glm::scale(Model, glm::vec3(0.5f));
	shader->SetUniformMat4f("model", Model);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	Model = glm::translate(glm::mat4(1), glm::vec3(2.0f, 0.0f, 1.0f));
	shader->SetUniformMat4f("model", Model);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));	
	Model = glm::translate(glm::mat4(1), glm::vec3(-1.0f, 0.0f, 2.0f));
	Model = glm::rotate(Model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	Model = glm::scale(Model, glm::vec3(0.25f));
	shader->SetUniformMat4f("model", Model);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	//光源
	objects[2]->shader->Bind();
	objects[2]->vao->Bind();
	Model = glm::translate(glm::mat4(1), objects[2]->pos);
	Model = glm::scale(Model, glm::vec3(0.1f));
	objects[2]->shader->SetUniformMat4f("model", Model);
	objects[2]->shader->SetUniformMat4f("view", m_View);
	objects[2]->shader->SetUniformMat4f("projection", m_Proj);
	objects[2]->shader->SetUniform3f("LightColor", glm::vec3(1));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}