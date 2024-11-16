#include "ParallaxMappingTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"

struct Vertex {
	glm::vec3 Pos;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
	glm::vec3 Tangent;
	glm::vec3 BiTangent;
};
void ParallaxMappingTest::CalcTB(Vertex& A, Vertex& B, Vertex& C)
{
	glm::vec3 edge1 = B.Pos - A.Pos;
	glm::vec3 edge2 = C.Pos - A.Pos;
	glm::vec2 deltaUV1 = B.TexCoord - A.TexCoord;
	glm::vec2 deltaUV2 = C.TexCoord - A.TexCoord;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	glm::vec3  tangent, bitangent;
	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent = glm::normalize(tangent);

	bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent = glm::normalize(bitangent);

	A.Tangent = tangent;
	B.Tangent = tangent;
	C.Tangent = tangent;
	A.BiTangent = bitangent;
	B.BiTangent = bitangent;
	C.BiTangent = bitangent;
}


ParallaxMappingTest::ParallaxMappingTest()
{
	vector<Vertex>Vertices = {
		{   glm::vec3(-1.0, 1.0, 0.0),		glm::vec3(0.0,	0.0, 1.0),	glm::vec2(0.0, 1.0) },//左上
		{	glm::vec3(-1.0, -1.0, 0.0),		glm::vec3(0.0,	0.0, 1.0),	glm::vec2(0.0, 0.0) },//左下
		{	glm::vec3(1.0, -1.0, 0.0),		glm::vec3(0.0,	0.0, 1.0),	glm::vec2(1.0, 0.0) },//右下
		{   glm::vec3(-1.0, 1.0, 0.0),		glm::vec3(0.0,	0.0, 1.0),	glm::vec2(0.0, 1.0) },//左上
		{	glm::vec3(1.0, -1.0, 0.0),		glm::vec3(0.0,	0.0, 1.0),	glm::vec2(1.0, 0.0) },//右下
		{	glm::vec3(1.0, 1.0, 0.0),		glm::vec3(0.0,	0.0, 1.0),	glm::vec2(1.0, 1.0) }//右上
	};
	CalcTB(Vertices[0], Vertices[1], Vertices[2]);
	CalcTB(Vertices[3], Vertices[4], Vertices[5]);
	{
        objects.push_back(make_shared<Object>());
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(Vertices.data(), sizeof(Vertex) * Vertices.size());
		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(2);
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(3);
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);
		objects[0]->texs.push_back(make_shared<Texture>("res/image/bricks2.jpg",true));
		objects[0]->texs.push_back(make_shared<Texture>("res/image/bricks2_normal.jpg"));
		objects[0]->texs.push_back(make_shared<Texture>("res/image/bricks2_disp.jpg"));
	}
	
	Shaders["Wall"] = make_shared<Shader>("res/shader/5AdvancedLighting/5ParallaxMapping/Wall.shader");

}


void ParallaxMappingTest::OnRender()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));

	Shaders["Wall"]->Bind();
	for (int i = 0; i < objects[0]->texs.size(); i++) {
		objects[0]->texs[i]->Bind(i);
	}
	objects[0]->vao->Bind();
	glm::mat4 modelMat = glm::mat4(1);
	Shaders["Wall"]->SetUniformMat4f("model", modelMat);
	Shaders["Wall"]->SetUniformMat4f("view", m_View);
	Shaders["Wall"]->SetUniformMat4f("projection", m_Proj);
	Shaders["Wall"]->SetUniform3f("LightPos", glm::vec3(0.5f, 1.0f, 0.3f));
	Shaders["Wall"]->SetUniform3f("ViewPos", Camera::GetInstance()->Position);
	Shaders["Wall"]->SetUniform1i("DiffuseMap", 0);
	Shaders["Wall"]->SetUniform1i("NormalMap", 1);
	Shaders["Wall"]->SetUniform1i("DepthMap", 2);
	Shaders["Wall"]->SetUniform1f("HeightScale",0.1);

	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}

void ParallaxMappingTest::OnImGuiRender()
{
}
