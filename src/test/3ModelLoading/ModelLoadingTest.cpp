#include "ModelLoadingTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"

#include"Model.h"
#include "imgui/imgui.h"
ModelLoadingTest::ModelLoadingTest()
{
	model = make_shared<Model>(GetResDir() + "res/model/nanosuit/nanosuit.obj");
	shader = make_shared<Shader>(GetResDir() + "res/shader/3ModelLoading/Default.shader");

	{//轴
		float axisVectices[] = {
			0.0f,0.0f,0.0f,
			10.f,0.f,0.f,
			0.0f,0.0f,0.0f,
			0.f,10.f,0.f,
			0.0f,0.0f,0.0f,
			0.f,0.f,10.f
		};
		objects.push_back(make_shared<Object>());

		objects[0]->shader = make_shared<Shader>(GetResDir() + "res/shader/axis.shader");
		objects[0]->shader->Bind();
		objects[0]->shader->SetUniformMat4f("model", glm::mat4(1));//默认静止，所以模型矩阵直接写死，不在渲染循环更新
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(axisVectices, sizeof(axisVectices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);//位置
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
	}
	{//光源
		float vertices[] = {
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
		objects.push_back(make_shared<Object>());

		objects[1]->shader = make_shared<Shader>(GetResDir() + "res/shader/light.shader");
		objects[1]->shader->Bind();
		
		objects[1]->vao = make_shared<VertexArray>();
		objects[1]->vbo = make_shared<VertexBuffer>(vertices, sizeof(vertices));
		objects[1]->layout = make_shared<VertexBufferLayout>();
		objects[1]->layout->Push<float>(3);//位置

		objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[1]->layout);
	}
}

void ModelLoadingTest::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	shader->Bind();
	glm::mat4 model_mat = glm::mat4(1);
	model_mat = glm::scale(model_mat,glm::vec3(0.2f));
	shader->SetUniformMat4f("model", model_mat);
	shader->SetUniformMat4f("view", m_View);
	shader->SetUniformMat4f("projection", m_Proj);
	shader->SetUniform3f("viewPos", Camera::GetInstance()->Position);
	shader->SetUniform1f("material.shininess", 32);
	// 定向光
	shader->SetUniform3f("dirLight.direction", dirLightdir);
	shader->SetUniform3f("dirLight.ambient", 0.05f * lightColor);
	shader->SetUniform3f("dirLight.diffuse", 0.8f * lightColor);
	shader->SetUniform3f("dirLight.specular", lightColor);
	//点光源
	for (int i = 0; i < 4; i++)
	{
		string IdxStr = "pointLights[" + to_string(i) + "].";
		shader->SetUniform3f(IdxStr + "position", pointLightPositions[i]);
		shader->SetUniform1f(IdxStr + "constant", 1.0f);
		shader->SetUniform1f(IdxStr + "linear", 0.09f);
		shader->SetUniform1f(IdxStr + "quadratic", 0.032f);
		shader->SetUniform3f(IdxStr + "ambient", 0.05f * lightColor);
		shader->SetUniform3f(IdxStr + "diffuse", 0.8f * lightColor);
		shader->SetUniform3f(IdxStr + "specular", lightColor);
	}
	//聚光
	shader->SetUniform3f("spotLight.position", Camera::GetInstance()->Position);
	shader->SetUniform3f("spotLight.direction", Camera::GetInstance()->Front);
	shader->SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader->SetUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(17.5)));
	shader->SetUniform1f("spotLight.constant", 1.0f);
	shader->SetUniform1f("spotLight.linear", 0.09f);
	shader->SetUniform1f("spotLight.quadratic", 0.032f);
	shader->SetUniform3f("spotLight.ambient", 0.05f * lightColor);
	shader->SetUniform3f("spotLight.diffuse", 0.8f * lightColor);
	shader->SetUniform3f("spotLight.specular", lightColor);


	model->DrawModel(shader);


	//坐标轴
	objects[0]->shader->Bind();
	objects[0]->vao->Bind();
	for (int i = 0; i < 3; i++) {
		glm::vec3 tmp = { 0,0,0 };
		tmp[i] = 1;
		objects[0]->shader->SetUniform3f("objectColor", tmp);
		objects[0]->shader->SetUniformMat4f("view", m_View);
		objects[0]->shader->SetUniformMat4f("projection", m_Proj);
		GLCall(glDrawArrays(GL_LINES, 2 * i, 2));
		GLCall(glLineWidth(3));
	}
	//光源
	objects[1]->shader->Bind();
	objects[1]->vao->Bind();
	objects[1]->shader->SetUniformMat4f("view", m_View);
	objects[1]->shader->SetUniformMat4f("projection", m_Proj);
	for (int i = 0; i < 4; i++) {
		glm::mat4 model = glm::translate(glm::mat4(1), pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.1f));

		objects[1]->shader->SetUniformMat4f("model", model);
		objects[1]->shader->SetUniform3f("LightColor", lightColor);

		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}
	

}	

void ModelLoadingTest::OnImGuiRender()
{
	ImGui::ColorEdit3("LightColor", &lightColor.x, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs);
	ImGui::DragFloat3("DirLightDir", &dirLightdir.x);
	ImGui::SliderFloat3("PointLightPos_1", &pointLightPositions[0].x, -10, 10);
	ImGui::SliderFloat3("PointLightPos_2", &pointLightPositions[1].x, -10, 10);
	ImGui::SliderFloat3("PointLightPos_3", &pointLightPositions[2].x, -10, 10);
	ImGui::SliderFloat3("PointLightPos_4", &pointLightPositions[3].x, -10, 10);
}
