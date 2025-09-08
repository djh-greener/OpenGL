#include "GammaCorrectionTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include "imgui/imgui.h"
#include "stb_image/stb_image.h"
unsigned int loadTexture(string path, bool gammaCorrection);
GammaCorrectionTest::GammaCorrectionTest()
{
	float planeVertices[] = {
		// positions            // normals         // texcoords
		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
		 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
	};
	{//µÿ∞Â
		objects.push_back(make_shared<Object>());
		objects[0]->shader = make_shared<Shader>(GetResDir() + "res/shader/5AdvancedLighting/2GammaCorrection/Floor.shader");
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(planeVertices, sizeof(planeVertices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(2);
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
		FloorTexture = loadTexture(GetResDir() + "res/image/wood.png", false);
		FloorTextureGamma = loadTexture(GetResDir() + "res/image/wood.png", true);
	}
	GLfloat cubeVertices[] = {
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
	{//π‚‘¥
		objects.push_back(make_shared<Object>());
		objects[1]->shader = make_shared<Shader>(GetResDir() + "res/shader/light.shader");
		objects[1]->vao = make_shared<VertexArray>();
		objects[1]->vbo = make_shared<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
		objects[1]->layout = make_shared<VertexBufferLayout>();
		objects[1]->layout->Push<float>(3);
		objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[1]->layout);
	}
}

void GammaCorrectionTest::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	objects[0]->shader->Bind();
	objects[0]->vao->Bind();
	objects[0]->shader->SetUniformMat4f("model", glm::mat4(1));
	objects[0]->shader->SetUniformMat4f("view", m_View);
	objects[0]->shader->SetUniformMat4f("projection", m_Proj);
	objects[0]->shader->SetUniform3f("LightPos", objects[1]->pos);
	objects[0]->shader->SetUniform3f("ViewPos", Camera::GetInstance()->Position);
	GLCall(glBindTexture(GL_TEXTURE_2D, GammaLoad ? FloorTextureGamma:FloorTexture));
	objects[0]->shader->SetUniform1i("FloorTexture", 0);
	objects[0]->shader->SetUniform1i("gamma", GammaCorrect);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

	objects[1]->shader->Bind();
	objects[1]->vao->Bind();
	glm::mat4 model = glm::translate(glm::mat4(1), objects[1]->pos);
	model = glm::scale(model, glm::vec3(0.1f));
	objects[1]->shader->SetUniformMat4f("model", model);
	objects[1]->shader->SetUniformMat4f("view", m_View);
	objects[1]->shader->SetUniformMat4f("projection", m_Proj);
	objects[1]->shader->SetUniform3f("LightColor", glm::vec3(1));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}

void GammaCorrectionTest::OnImGuiRender()
{
	if (ImGui::Button("GammaLoad"))
		GammaLoad = !GammaLoad;
	string GammaLoadStr = GammaLoad ? "ON" : "OFF";
	ImGui::SameLine(100);
	ImGui::Text("GammaLoad is %s", GammaLoadStr.c_str());

	if (ImGui::Button("GammaCorrect"))
		GammaCorrect = !GammaCorrect;
	string GammaCorrectStr = GammaCorrect ? "ON" : "OFF";
	ImGui::SameLine(100);
	ImGui::Text("GammaCorrect is %s", GammaCorrectStr.c_str());

	ImGui::SliderFloat3("LightPos", &objects[1]->pos.x, -10, 10);
}

unsigned int loadTexture(string path, bool gammaCorrection)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	GLenum internalFormat;
	GLenum dataFormat;
	if (data)
	{

		if (nrComponents == 1)
		{
			internalFormat = dataFormat = GL_RED;
		}
		else if (nrComponents == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}