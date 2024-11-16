#pragma once
#include"glm/glm.hpp"
#include<memory>
class Shader;
class VertexArray;
class VertexBuffer;
class VertexBufferLayout;
class Texture;
//初级渲染模型，后期要建立材质系统
struct Object
{
	std::shared_ptr<Shader>shader;
	std::shared_ptr<VertexArray>vao;
	std::shared_ptr<VertexBuffer> vbo;
	std::shared_ptr<VertexBufferLayout> layout;
	std::vector<std::shared_ptr<Texture>>texs;
	glm::vec3 pos = { 0, 0, 0};
	

};

