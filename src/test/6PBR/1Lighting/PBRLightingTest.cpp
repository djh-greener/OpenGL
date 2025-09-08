#include "PBRLightingTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include "Geometry.h"
#include "IndexBuffer.h"
#include "imgui/imgui.h"

PBRLightingTest::PBRLightingTest()
{
	{
		vector<unsigned int> indices;
		vector<float> data;
		renderSphere(data, indices,64,64);
		objects.push_back(make_shared<Object>());
		Shaders["Sphere"] = make_shared<Shader>(GetResDir() + "res/shader/6PBR/1Lighting/Sphere.shader");
		Shaders["SphereTexture"] = make_shared<Shader>(GetResDir() + "res/shader/6PBR/1Lighting/SphereTexture.shader");
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(data.data(), sizeof(float)*data.size());

		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(2);

		ebo = make_shared<IndexBuffer>(indices.data(), indices.size());
		ebo->Bind();
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);

		objects[0]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/rusted_iron/albedo.png",true));
		objects[0]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/rusted_iron/ao.png"));
		objects[0]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/rusted_iron/metallic.png"));
		objects[0]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/rusted_iron/normal.png"));
		objects[0]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/rusted_iron/roughness.png"));
		for (size_t i = 0; i < objects[0]->texs.size(); i++)
			objects[0]->texs[i]->Bind(i);
		
		objects[0]->pos = glm::vec3(0, 0, 0);
	}


	nrRows = 7;
	nrCols = 7;
	lightPositions = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	lightColors = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};
	nrLights = 4;
}

void PBRLightingTest::OnRender()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));

	string CurrentShader = RenderTextures ? "SphereTexture" : "Sphere";
	Shaders[CurrentShader]->Bind();
	objects[0]->vao->Bind();
	ebo->Bind();

	Shaders[CurrentShader]->SetUniformMat4f("view", m_View);
	Shaders[CurrentShader]->SetUniformMat4f("projection", m_Proj);
	Shaders[CurrentShader]->SetUniform3f("ViewPos", Camera::GetInstance()->Position); 
	for (int i = 0; i < nrLights; i++) {
		string str = "Lights[" + to_string(i) + "].";
		Shaders[CurrentShader]->SetUniform3f(str + "Position", lightPositions[i]);
		Shaders[CurrentShader]->SetUniform3f(str + "Color", lightColors[i]);
	}

	if (RenderTextures)
	{
		Shaders[CurrentShader]->SetUniform1i("albedoMap", 0);
		Shaders[CurrentShader]->SetUniform1i("aoMap", 1);
		Shaders[CurrentShader]->SetUniform1i("metallicMap", 2);
		Shaders[CurrentShader]->SetUniform1i("normalMap", 3);
		Shaders[CurrentShader]->SetUniform1i("roughnessMap", 4);
		float space = 3;
		for (float row = 0; row < nrRows; row++) {
			for (float col = 0; col < nrCols; col++) {
				glm::mat4 model = glm::mat4(1);
				float x = (col - nrCols / 2) * space;
				float y = (row - nrRows / 2) * space;
				model = glm::translate(model, glm::vec3(x, y, 0));
				Shaders[CurrentShader]->SetUniformMat4f("model", model);;
				GLCall(glDrawElements(GL_TRIANGLE_STRIP, ebo->GetCount(), GL_UNSIGNED_INT, 0));
			}
		}
	}
	else
	{
		Shaders[CurrentShader]->SetUniform3f("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
		float space = 3;
		for (float row = 0; row < nrRows; row++) {
			Shaders[CurrentShader]->SetUniform1f("metallic", row / nrRows);
			for (float col = 0; col < nrCols; col++) {
				glm::mat4 model = glm::mat4(1);
				float x = (col - nrCols / 2) * space;
				float y = (row - nrRows / 2) * space;
				model = glm::translate(model, glm::vec3(x, y, 0));
				Shaders[CurrentShader]->SetUniformMat4f("model", model);
				Shaders[CurrentShader]->SetUniform1f("roughness", glm::clamp(col / nrCols, 0.05f, 1.0f));
				Shaders[CurrentShader]->SetUniform1f("ao", 1.f);
				GLCall(glDrawElements(GL_TRIANGLE_STRIP, ebo->GetCount(), GL_UNSIGNED_INT, 0));
			}
		}
	}

}

void PBRLightingTest::OnImGuiRender()
{
	if (ImGui::Button("RenderWithTextures"))
		RenderTextures = !RenderTextures;
	ImGui::SameLine(300);
	string RenderWithTextures = RenderTextures ? "On" : "OFF";
	ImGui::Text(RenderWithTextures.c_str());
}


