#pragma once
#include<iostream>
#include<vector>
#include<memory>
using namespace std;
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Shader;
class Texture;
class Mesh;

struct MeshNode {
    vector<shared_ptr<Mesh> >meshes;
    vector< shared_ptr<MeshNode>>children;
};

class Model
{
public:
    Model(string path) {
        loadModel(path);
    }
	void DrawModel(shared_ptr<Shader> shader) { DrawModelRecursive(RootNode, shader); }
	//返回Mesh树的第一个Mesh
	shared_ptr<Mesh>GetMesh();
private:
    shared_ptr<MeshNode>RootNode;//使用同一个着色器的网格树
    string directory;//模型所在目录
    vector<shared_ptr<Texture>> textures_loaded;//全局的已加载纹理

    void loadModel(string path);//可以作为虚函数
    void processNode(aiNode* ainode, shared_ptr<MeshNode> node, const aiScene* scene);
    shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    vector< shared_ptr<Texture>> loadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName);

	void DrawModelRecursive(shared_ptr<MeshNode> node, shared_ptr<Shader> shader);
	shared_ptr<Mesh>GetMeshRecursive(shared_ptr<MeshNode> node);
};
