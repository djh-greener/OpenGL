#pragma once
#include"glm/glm.hpp"
#include<memory>
#include<vector>
using namespace std;
class Shader;
class VertexArray;
class VertexBuffer;
class IndexBuffer;
class VertexBufferLayout;
class Texture;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 BiTangent;
};
class Mesh
{
public:
   shared_ptr<VertexArray> vao;
   shared_ptr<VertexBuffer> vbo;
   shared_ptr<IndexBuffer>ebo;
   shared_ptr<VertexBufferLayout> layout;

   void SetupMesh();

    vector<Vertex>vertices;
    vector<unsigned int>indices;
    vector< shared_ptr<Texture>> textures;
    

    Mesh(vector<Vertex>vertices, vector<unsigned int> indices, vector< shared_ptr<Texture>> textures);
    void DrawMesh(shared_ptr<Shader> shader);

};

