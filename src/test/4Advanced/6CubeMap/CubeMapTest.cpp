#include "CubeMapTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include"Model.h"

#include "stb_image/stb_image.h"
#include "imgui/imgui.h"


CubeMapTest::CubeMapTest()
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	{
		objects.push_back(make_shared<Object>());
		objects[0]->shader = make_shared<Shader>(GetResDir() + "res/shader/4Advanced/6CubeMap/SkyBox.shader");
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(skyboxVertices, sizeof(skyboxVertices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
	}

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	{
		objects.push_back(make_shared<Object>());
		objects[1]->shader = make_shared<Shader>(GetResDir() + "res/shader/4Advanced/6CubeMap/ReflectCube.shader");
		objects[1]->vao = make_shared<VertexArray>();
		objects[1]->vbo = make_shared<VertexBuffer>(vertices, sizeof(vertices));
		objects[1]->layout = make_shared<VertexBufferLayout>();
		objects[1]->layout->Push<float>(3);
		objects[1]->layout->Push<float>(3);
		objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[1]->layout);
	}

	float quadVertices[] = {
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};
	{//四边形
		objects.push_back(make_shared<Object>());
		objects[2]->shader = make_shared<Shader>(GetResDir() + "res/shader/4Advanced/6CubeMap/Plane.shader");
		objects[2]->vao = make_shared<VertexArray>();
		objects[2]->vbo = make_shared<VertexBuffer>(quadVertices, sizeof(quadVertices));
		objects[2]->layout = make_shared<VertexBufferLayout>();
		objects[2]->layout->Push<float>(2);
		objects[2]->layout->Push<float>(2);
		objects[2]->vao->AddBuffer(*objects[2]->vbo, *objects[2]->layout);
		objects[2]->texs.push_back(make_shared<Texture>(GetResDir() + "res/image/metal.png"));
	}

	model = make_shared<Model>(GetResDir() + "res/model/nanosuit/nanosuit.obj");
	ReflectShader = make_shared<Shader>(GetResDir() + "res/shader/4Advanced/6CubeMap/ReflectNanosuit.shader");
	RefractShader = make_shared<Shader>(GetResDir() + "res/shader/4Advanced/6CubeMap/RefractNanosuit.shader");
	DefaultShader = make_shared<Shader>(GetResDir() + "res/shader/4Advanced/6CubeMap/Nanosuit.shader");
	vector<std::string> faces
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};
	for (auto &it : faces) 
		it = GetResDir() + "res/image/skybox/" + it;
	LoadCubemap(faces);//此纹理占用8号纹理槽
}

void CubeMapTest::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glDepthFunc(GL_LEQUAL));

	//立方体贴图
	objects[1]->shader->Bind();
	objects[1]->vao->Bind();
	objects[1]->shader->SetUniform1i("SkyBox", SkyBoxIdx);
	GLCall(glActiveTexture(GL_TEXTURE0 + SkyBoxIdx));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));
	objects[1]->shader->SetUniformMat4f("projection", m_Proj);
	objects[1]->shader->SetUniformMat4f("view", m_View);
	objects[1]->shader->SetUniform3f("CameraPos", Camera::GetInstance()->Position);
	objects[1]->shader->SetUniformMat4f("model", glm::translate(glm::mat4(1), glm::vec3(2, 1, 2)));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

	objects[2]->shader->Bind();
	objects[2]->vao->Bind();
	objects[2]->texs[0]->Bind(0);
	glm::mat4 model_mat = glm::rotate(glm::mat4(1), glm::radians(90.f), glm::vec3(1, 0, 0));
	model_mat = glm::scale(model_mat, glm::vec3(3));
	objects[2]->shader->SetUniformMat4f("model", model_mat);
	objects[2]->shader->SetUniformMat4f("projection", m_Proj);
	objects[2]->shader->SetUniformMat4f("view", m_View);
	objects[2]->shader->SetUniform1i("screenTexture", 0);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

	ReflectShader->Bind();
	float radians = glm::radians(glfwGetTime()*10);
	glm::mat4 ScaleDown = glm::scale(glm::mat4(1),glm::vec3(0.2f));
	ReflectShader->SetUniformMat4f("model", glm::rotate(ScaleDown, radians,glm::vec3(0,1,0)));
	ReflectShader->SetUniformMat4f("projection", m_Proj);
	ReflectShader->SetUniformMat4f("view", m_View);
	ReflectShader->SetUniform3f("viewPos", Camera::GetInstance()->Position);
	ReflectShader->SetUniform1i("SkyBox", SkyBoxIdx);
	GLCall(glActiveTexture(GL_TEXTURE0 + SkyBoxIdx));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));
	model->DrawModel(ReflectShader);

	{
		RefractShader->Bind();
		//由于这些函数是右乘，所以实际顺序是 旋转，平移，缩放
		glm::mat4 model_mat = glm::scale(glm::mat4(1), glm::vec3(0.2f));
		model_mat = glm::translate(model_mat, glm::vec3(5, 0, 5));
		model_mat = glm::rotate(model_mat, radians, glm::vec3(0, 1, 0));
		
		RefractShader->SetUniformMat4f("model", model_mat);
		RefractShader->SetUniformMat4f("projection", m_Proj);
		RefractShader->SetUniformMat4f("view", m_View);
		RefractShader->SetUniform3f("viewPos", Camera::GetInstance()->Position);
		RefractShader->SetUniform1i("SkyBox", SkyBoxIdx);
		//GLCall(glActiveTexture(GL_TEXTURE0 + SkyBoxIdx));
		//GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));
		model->DrawModel(RefractShader);
	}
	{
		DefaultShader->Bind();
		//由于这些函数是右乘，所以实际顺序是 旋转，平移，缩放
		glm::mat4 model_mat = glm::scale(glm::mat4(1), glm::vec3(0.2f));
		model_mat = glm::translate(model_mat, glm::vec3(-5, 0, -5));
		model_mat = glm::rotate(model_mat, radians, glm::vec3(0, 1, 0));

		DefaultShader->SetUniformMat4f("model", model_mat);
		DefaultShader->SetUniformMat4f("projection", m_Proj);
		DefaultShader->SetUniformMat4f("view", m_View);
		DefaultShader->SetUniform3f("viewPos", Camera::GetInstance()->Position);
		DefaultShader->SetUniform1i("SkyBox", SkyBoxIdx);
		//GLCall(glActiveTexture(GL_TEXTURE0 + SkyBoxIdx));
		//GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));
		model->DrawModel(DefaultShader);
	}
	//天空盒
	objects[0]->shader->Bind();
	objects[0]->vao->Bind();
	objects[0]->shader->SetUniformMat4f("projection", m_Proj);
	objects[0]->shader->SetUniformMat4f("view", glm::mat4(glm::mat3(m_View)));
	objects[0]->shader->SetUniform1i("SkyBox", SkyBoxIdx);
	GLCall(glActiveTexture(GL_TEXTURE0 + SkyBoxIdx));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	
}

void CubeMapTest::OnImGuiRender()
{
}

void CubeMapTest::LoadCubemap(vector<string> faces)
{

	GLCall(glGenTextures(1, &textureID));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
		}
		else 
			Log::GetInstance() << Log::Level::Warning << "Cubemap texture failed to load at path: " << faces[i] << endl;
		
		stbi_image_free(data);
	}
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

}
