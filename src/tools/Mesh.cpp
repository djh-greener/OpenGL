#include "Mesh.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include"IndexBuffer.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
void Mesh::SetupMesh()
{
    vao = make_shared<VertexArray>();
    vbo = make_shared<VertexBuffer>(vertices.data(), sizeof(Vertex) * vertices.size());
    layout = make_shared<VertexBufferLayout>();
    layout->Push<float>(3);//Position
    layout->Push<float>(3);//Normal
	layout->Push<float>(2);
	layout->Push<float>(3);
	layout->Push<float>(3);

    ebo = make_shared<IndexBuffer>(indices.data(), indices.size());
    ebo->Bind();
    vao->AddBuffer(*vbo, *layout);
    vao->UnBind();
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<shared_ptr<Texture>> textures)
{
    this->vertices = vertices;
    this->textures = textures;
    this->indices = indices;
    SetupMesh();
}

void Mesh::DrawMesh(shared_ptr<Shader> shader)
{
    unsigned int diffuseNr=0 ,specularNr= 0, ambientNr = 0, normalNr = 0;
    for (int i = 0; i < textures.size(); i++) {
        textures[i]->Bind(i);
        shader->Bind();
        string varName = "material." + textures[i]->type;
		//如果着色器中没有定义对应uniform变量，有警告但能跑
        if (textures[i]->type == "diffuse")
            varName += to_string(++diffuseNr);
        else if (textures[i]->type == "specular")
            varName += to_string(++specularNr);
		else if (textures[i]->type == "ambient")
			varName += to_string(++ambientNr);
		else if (textures[i]->type == "normal")
			varName += to_string(++normalNr);
        else
            Log::GetInstance() << Log::Level::Warning << "Error Texture Type!" << endl;
        shader->SetUniform1i(varName, i);
    }
    vao->Bind();
	//GLCall(glDrawArrays(GL_TRIANGLES, 0 , vertices.size()));

	ebo->Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ebo->GetCount(), GL_UNSIGNED_INT, 0));
    vao->UnBind();
}

