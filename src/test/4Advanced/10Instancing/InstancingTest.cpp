#include "InstancingTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include"Model.h"
#include"Mesh.h"
#include"IndexBuffer.h"

#include "imgui/imgui.h"
#include "stb_image/stb_image.h"

unsigned int LoadHDR(string path) {


	float* data;
	int width, height, nrComponents;
	data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);

	GLuint texHDR;
	glGenTextures(1, &texHDR);
	glBindTexture(GL_TEXTURE_2D, texHDR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGB, GL_FLOAT, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
	return texHDR;
}

InstancingTest::InstancingTest()
{
	{
		//矩形顶点
		float quadVertices[] = {
			// 位置          // 颜色
			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
		};
		// 矩形偏移
		int index = 0;
		float offset = 0.1f;
		for (int y = -10; y < 10; y += 2)
		{
			for (int x = -10; x < 10; x += 2)
			{
				glm::vec2 translation;
				translation.x = (float)x / 10.0f + offset;
				translation.y = (float)y / 10.0f + offset;
				translations[index++] = translation;
			}
		}
		objects.push_back(make_shared<Object>());
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(quadVertices, sizeof(quadVertices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(2);
		objects[0]->layout->Push<float>(3);
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
		//需要另一vbo，所以生成了一个Object，两个vbo都绑定到objects[0]->vao
		objects.push_back(make_shared<Object>());
		objects[1]->vbo = make_shared<VertexBuffer>(translations, sizeof(translations));
		//这里不得不调用OpenGL的API，因为我的vao根据VertexBufferLayout自动计算第i个属性，但是这里需要直接指定为第二个属性
		GLCall(glEnableVertexAttribArray(2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
		GLCall(glVertexAttribDivisor(2, 1));
	}
	{
		//小行星 位置数据准备
		
		modelMatrices = vector<glm::mat4>(amount);
		float radius = 100.0;
		float offset = 40.f;
		for (unsigned int i = 0; i < amount; i++)
		{
			glm::mat4 model = glm::mat4(1);
			
			// 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
			float angle = (float)i / (float)amount * 360.0f;
			float displacement = (rand() % (int)(offset * 100)) / 100.0f - offset;
			float x = sin(angle) * radius + displacement;
			displacement = (rand() % (int)(offset * 100)) / 100.0f - offset;
			float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
			displacement = (rand() % (int)(offset * 100)) / 100.0f - offset;
			float z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. 缩放：在 0.05 和 0.25f 之间缩放
			float scale = (rand() % 20) / 100.0f + 0.05;
			model = glm::scale(model, glm::vec3(scale));

			// 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
			float rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			// 4. 添加到矩阵的数组中
			modelMatrices[i] = model;
		}
		//小行星绘制对象准备
		
		planet = make_shared<Model>("res/model/planet/planet.obj");
		rock = make_shared<Model>("res/model/rock/rock.obj");
		sphere = make_shared<Model>("res/model/base/sphere.obj");
		texHDR = LoadHDR("res/image/space2.hdr");

		shared_ptr<VertexBuffer>vbo = make_shared<VertexBuffer>(modelMatrices.data(), sizeof(glm::mat4) * modelMatrices.size());
		
		auto vao = rock->GetMesh()->vao;
		vao->Bind();
		vbo->Bind();
		GLsizei vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		vao->UnBind();
	}
	{//生成一系列着色器
		vector<string>shadernames = { "Instancing","InstancingArray","UnInstancedPlanet","Background","InstancedPlanet" };
		string directory = "res/shader/4Advanced/10Instancing/";
		for (auto name : shadernames) {
			shaders[name] = make_shared<Shader>(directory + name + ".shader");
		}
	}


}

void InstancingTest::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	//GLCall(glDepthFunc(GL_LEQUAL));//渲染背景时需要
	
	if(CurrentShader=="Instancing")
	{
		objects[0]->vao->Bind();
		shaders[CurrentShader]->Bind();
		for (int idx = 0; idx < 100; idx++) 
			shaders[CurrentShader]->SetUniform2f("offsets[" + to_string(idx) + "]", translations[idx]);
		GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100));
	}
	else if (CurrentShader == "InstancingArray") 
	{
		objects[0]->vao->Bind();
		shaders[CurrentShader]->Bind();
		GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100));
	}
	else if (CurrentShader == "UnInstancedPlanet") 
	{//小行星和星球用同一个着色器
		glDepthMask(GL_FALSE);
		//天空盒背景
		shaders["Background"]->Bind();
		shaders["Background"]->SetUniformMat4f("view", glm::mat4(glm::mat3(m_View)));
		shaders["Background"]->SetUniformMat4f("projection", m_Proj);
		glBindTexture(GL_TEXTURE_2D, texHDR);
		shaders["Background"]->SetUniform1i("screenTexture", 0);
		sphere->DrawModel(shaders["Background"]);
		glDepthMask(GL_TRUE);

		//星球
		shaders["UnInstancedPlanet"]->Bind();
		float radians = glm::radians(glfwGetTime() * 10);
		glm::mat4 model = glm::rotate(glm::mat4(1), radians, glm::vec3(0, 1, 0.2));
		model = glm::scale(model, glm::vec3(4, 4, 4));
		shaders["UnInstancedPlanet"]->SetUniformMat4f("model", model);
		shaders["UnInstancedPlanet"]->SetUniformMat4f("view", m_View);
		shaders["UnInstancedPlanet"]->SetUniformMat4f("projection", m_Proj);
		shaders["UnInstancedPlanet"]->SetUniform3f("viewPos", Camera::GetInstance()->Position);
		shaders["UnInstancedPlanet"]->SetUniform3f("dirLight.direction", glm::vec3(1, 1, -1));
		shaders["UnInstancedPlanet"]->SetUniform3f("dirLight.lightColor", glm::vec3(1, 1, 1));
		shaders["UnInstancedPlanet"]->SetUniform1f("material.shinness", 4);
		planet->DrawModel(shaders["UnInstancedPlanet"]);
		//小行星
		for (int i = 0; i < modelMatrices.size(); i++) {
			shaders["UnInstancedPlanet"]->SetUniformMat4f("model", modelMatrices[i]);
			rock->DrawModel(shaders["UnInstancedPlanet"]);
		}
	}
	else if (CurrentShader == "InstancedPlanet")
	{
		glDepthMask(GL_FALSE);
		//天空盒背景
		shaders["Background"]->Bind();
		shaders["Background"]->SetUniformMat4f("view", glm::mat4(glm::mat3(m_View)));
		shaders["Background"]->SetUniformMat4f("projection", m_Proj);
		glBindTexture(GL_TEXTURE_2D, texHDR);
		shaders["Background"]->SetUniform1i("screenTexture", 0);
		sphere->DrawModel(shaders["Background"]);
		glDepthMask(GL_TRUE);

		//星球
		shaders["UnInstancedPlanet"]->Bind();
		float radians = glm::radians(glfwGetTime() * 10);
		glm::mat4 model = glm::rotate(glm::mat4(1), radians, glm::vec3(0, 1, 0.2));
		model = glm::scale(model, glm::vec3(4, 4, 4));
		shaders["UnInstancedPlanet"]->SetUniformMat4f("model", model);
		shaders["UnInstancedPlanet"]->SetUniformMat4f("view", m_View);
		shaders["UnInstancedPlanet"]->SetUniformMat4f("projection", m_Proj);
		shaders["UnInstancedPlanet"]->SetUniform3f("viewPos", Camera::GetInstance()->Position);
		shaders["UnInstancedPlanet"]->SetUniform3f("dirLight.direction", glm::vec3(1, 1, -1));
		shaders["UnInstancedPlanet"]->SetUniform3f("dirLight.lightColor", glm::vec3(1, 1, 1));
		shaders["UnInstancedPlanet"]->SetUniform1f("material.shinness", 4);
		planet->DrawModel(shaders["UnInstancedPlanet"]);
		//小行星
		//采用实例化数组可以渲染大量实例同时保持高性能，不过将模型变换矩阵嵌入顶点属性，意味着无法实时更新。比如说让小行星绕圆环中心旋转就无法达成
		shaders["InstancedPlanet"]->Bind();
		shaders["InstancedPlanet"]->SetUniformMat4f("view", m_View);
		shaders["InstancedPlanet"]->SetUniformMat4f("projection", m_Proj);
		shaders["InstancedPlanet"]->SetUniform3f("viewPos", Camera::GetInstance()->Position);
		shaders["InstancedPlanet"]->SetUniform3f("dirLight.direction", glm::vec3(1, 1, -1));
		shaders["InstancedPlanet"]->SetUniform3f("dirLight.lightColor", glm::vec3(1, 1, 1));
		shaders["InstancedPlanet"]->SetUniform1f("material.shinness", 4);
		rock->GetMesh()->vao->Bind();
		rock->GetMesh()->ebo->Bind();

		GLCall(glDrawElementsInstanced(GL_TRIANGLES, rock->GetMesh()->ebo->GetCount(), GL_UNSIGNED_INT, 0, amount));
	}
}

void InstancingTest::OnImGuiRender()
{
	ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);

	if (ImGui::Button("Instancing")) 
		CurrentShader = "Instancing";
	if (ImGui::Button("InstancingArray"))
		CurrentShader = "InstancingArray";
	if (ImGui::Button("UnInstancedPlanet"))
		CurrentShader = "UnInstancedPlanet";
	if (ImGui::Button("InstancedPlanet"))
		CurrentShader = "InstancedPlanet";
	
}


