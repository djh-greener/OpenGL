#include "PointShadowsTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include"FrameBuffer.h"
#include "imgui/imgui.h"
extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

PointShadowsTest::PointShadowsTest()
{
	GLCall(glGenTextures(1, &DepthCubeMap));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, DepthCubeMap));
	for (unsigned int i = 0; i < 6; i++) {
		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
	}
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

	GLCall(glGenFramebuffers(1, &DepthFbo));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, DepthFbo));
	GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, DepthCubeMap, 0));
	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glReadBuffer(GL_NONE));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

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
	{//Cube
		objects.push_back(make_shared<Object>());
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(CubeVertices, sizeof(CubeVertices));
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(2);
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
		objects[0]->texs.push_back(make_shared<Texture>("res/image/wood.png", true));
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
		objects[1]->shader = make_shared<Shader>("res/shader/light.shader");
		objects[1]->vao = make_shared<VertexArray>();
		objects[1]->vbo = make_shared<VertexBuffer>(LightVertices, sizeof(LightVertices));
		objects[1]->layout = make_shared<VertexBufferLayout>();
		objects[1]->layout->Push<float>(3);
		objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[1]->layout);
		objects[1]->pos = glm::vec3(0);
	}
	{//用于显示深度贴图的天空盒
		objects.push_back(make_shared<Object>());
		objects[2]->vao = make_shared<VertexArray>();
		objects[2]->vao->AddBuffer(*objects[1]->vbo, *objects[1]->layout);
	}
	float axisVectices[] = {
	0.0f,	0.0f,	0.0f,
	10.f,	0.f,	0.f,
	0.0f,	0.0f,	0.0f,
	0.f,	10.f,	0.f,
	0.0f,	0.0f,	0.0f,
	0.f,	0.f,	10.f
	};
	{//轴

		objects.push_back(make_shared<Object>());

		objects[3]->shader = make_shared<Shader>("res/shader/axis.shader");
		objects[3]->shader->Bind();
		objects[3]->shader->SetUniformMat4f("model", glm::mat4(1));//默认静止，所以模型矩阵直接写死，不在渲染循环更新
		objects[3]->vao = make_shared<VertexArray>();
		objects[3]->vbo = make_shared<VertexBuffer>(axisVectices, sizeof(axisVectices));
		objects[3]->layout = make_shared<VertexBufferLayout>();
		objects[3]->layout->Push<float>(3);//位置
		objects[3]->vao->AddBuffer(*objects[3]->vbo, *objects[3]->layout);
	}
	vector<string>shadernames = { "Scene","DepthBox","Depth"};//"Scene"
	string directory = "res/shader/5AdvancedLighting/3Shadows/2PointShadows/";
	for (auto name : shadernames) {
		Shaders[name] = make_shared<Shader>(directory + name + ".shader");
	}
}

void PointShadowsTest::OnRender()
{
	GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, DepthFbo));
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
	GLfloat near = 1.0f;
	GLfloat far = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
	vector<glm::mat4> ShadowTransforms;
	ShadowTransforms.push_back(shadowProj *	glm::lookAt(objects[1]->pos, objects[1]->pos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	ShadowTransforms.push_back(shadowProj *	glm::lookAt(objects[1]->pos, objects[1]->pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	ShadowTransforms.push_back(shadowProj *	glm::lookAt(objects[1]->pos, objects[1]->pos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	ShadowTransforms.push_back(shadowProj *	glm::lookAt(objects[1]->pos, objects[1]->pos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	ShadowTransforms.push_back(shadowProj *	glm::lookAt(objects[1]->pos, objects[1]->pos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	ShadowTransforms.push_back(shadowProj *	glm::lookAt(objects[1]->pos, objects[1]->pos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
	Shaders["Depth"]->Bind();
	for (int i = 0; i < 6; i++)
		Shaders["Depth"]->SetUniformMat4f("ShadowMatrices[" + to_string(i) + "]", ShadowTransforms[i]);
	Shaders["Depth"]->SetUniform3f("LightPos", objects[1]->pos);
	Shaders["Depth"]->SetUniform1f("FarPlane", far);
	RenderScene(Shaders["Depth"]);
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
	if (DrawDepth) {
		GLCall(glDepthFunc(GL_LEQUAL));
		objects[2]->vao->Bind();
		Shaders["DepthBox"]->Bind();
		Shaders["DepthBox"]->SetUniformMat4f("view", glm::mat4(glm::mat3(m_View)));
		Shaders["DepthBox"]->SetUniformMat4f("projection", m_Proj);
		Shaders["DepthBox"]->SetUniform1i("SkyBox", 0);
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, DepthCubeMap));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}
	else {
		Shaders["Scene"]->Bind();
		Shaders["Scene"]->SetUniformMat4f("view", m_View);
		Shaders["Scene"]->SetUniformMat4f("projection", m_Proj);
		Shaders["Scene"]->SetUniform1i("Diffuse", 0);
		Shaders["Scene"]->SetUniform1i("DepthMap", 8);

		Shaders["Scene"]->SetUniform3f("LightPos", objects[1]->pos);
		Shaders["Scene"]->SetUniform3f("ViewPos", Camera::GetInstance()->Position);
		Shaders["Scene"]->SetUniform1f("FarPlane", far);

		RenderScene(Shaders["Scene"]);
	}
}

void PointShadowsTest::OnImGuiRender()
{
	ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
	if (ImGui::Button("DrawDepth"))
		DrawDepth = !DrawDepth;
	ImGui::SliderFloat3("LightPos", &objects[1]->pos.x, -10, 10);
}

void PointShadowsTest::RenderScene(shared_ptr<Shader>shader)
{
	glm::mat4 model = glm::mat4(1);

	objects[0]->vao->Bind();
	objects[0]->texs[0]->Bind(0);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_CUBE_MAP, DepthCubeMap);
	//Room
	model = glm::scale(glm::mat4(1), glm::vec3(10));
	shader->SetUniform1i("ReverseNormal", true);
	shader->SetUniformMat4f("model", model);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	shader->SetUniform1i("ReverseNormal", false);

	//Cubes
	model = glm::translate(glm::mat4(1), glm::vec3(4.0f, -3.5f, 0.0));
	shader->SetUniformMat4f("model", model);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

	model = glm::translate(glm::mat4(1), glm::vec3(2.0f, 3.0f, 1.0));
	model = glm::scale(model, glm::vec3(1.5));
	shader->SetUniformMat4f("model", model);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

	model = glm::translate(glm::mat4(1), glm::vec3(-3.0f, -1.0f, 0.0));
	shader->SetUniformMat4f("model", model);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

	model = glm::translate(glm::mat4(1), glm::vec3(-1.5f, 1.0f, 1.5));
	shader->SetUniformMat4f("model", model);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

	model = glm::translate(glm::mat4(1), glm::vec3(-1.5f, 2.0f, -3.0));
	model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(1.5));
	shader->SetUniformMat4f("model", model);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	//Light
	model = glm::translate(glm::mat4(1), objects[1]->pos);
	model = glm::scale(model, glm::vec3(0.1));
	objects[1]->shader->Bind();
	objects[1]->shader->SetUniformMat4f("model",model);
	objects[1]->shader->SetUniformMat4f("view",m_View);
	objects[1]->shader->SetUniformMat4f("projection",m_Proj);
	objects[1]->shader->SetUniform3f("LightColor", glm::vec3(1));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	//axis
	objects[3]->shader->Bind();
	objects[3]->vao->Bind();
	for (int i = 0; i < 3; i++) {
		glm::vec3 tmp = { 0,0,0 };
		tmp[i] = 1;
		objects[3]->shader->SetUniform3f("objectColor", tmp);
		objects[3]->shader->SetUniformMat4f("view", m_View);
		objects[3]->shader->SetUniformMat4f("projection", m_Proj);
		GLCall(glDrawArrays(GL_LINES, 2 * i, 2));
		GLCall(glLineWidth(3));
	}
}