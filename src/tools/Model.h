#pragma once
#include<iostream>
#include<vector>
#include<memory>
using namespace std;
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include<glm/glm.hpp>

class Shader;
class Texture;
class Mesh;

struct MeshNode {
    vector<shared_ptr<Mesh> >meshes;
    vector< shared_ptr<MeshNode>>children;
};

class Model
{
    using Float = std::numeric_limits<float>;
public:
    Model(string path, bool calcVertexRange_ = false):calcVertexRange(calcVertexRange_){
        maxVertex = { Float::min(),Float::min(),Float::min() };
        minVertex = { Float::max(),Float::max(), Float::max() };
        loadModel(path);
    }
	void DrawModel(shared_ptr<Shader> shader) { DrawModelRecursive(RootNode, shader); }
	//����Mesh���ĵ�һ��Mesh
	shared_ptr<Mesh>GetMesh();
    glm::vec3 maxVertex;
    glm::vec3 minVertex;
    bool calcVertexRange = false;
private:
    shared_ptr<MeshNode>RootNode;//ʹ��ͬһ����ɫ����������
    string directory;//ģ������Ŀ¼
    vector<shared_ptr<Texture>> textures_loaded;//ȫ�ֵ��Ѽ�������

    void loadModel(string path);//������Ϊ�麯��
    void processNode(aiNode* ainode, shared_ptr<MeshNode> node, const aiScene* scene);
    shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    vector< shared_ptr<Texture>> loadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName);

	void DrawModelRecursive(shared_ptr<MeshNode> node, shared_ptr<Shader> shader);
	shared_ptr<Mesh>GetMeshRecursive(shared_ptr<MeshNode> node);
};
