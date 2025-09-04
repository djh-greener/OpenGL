#include "Model.h"
#include"Mesh.h"
#include"Shader.h"
#include"Texture.h"
#include"MyDebug.h"

void Model::DrawModelRecursive(shared_ptr<MeshNode> node, shared_ptr<Shader> shader)
{
	for (auto mesh: node->meshes)
		mesh->DrawMesh(shader);
	for (auto childnode : node->children) {
		DrawModelRecursive(childnode, shader);
	}
}

shared_ptr<Mesh> Model::GetMeshRecursive(shared_ptr<MeshNode> node)
{
	for (auto mesh : node->meshes)
		return mesh;
	for (auto childnode : node->children)
		return GetMeshRecursive(childnode);
}

shared_ptr<Mesh> Model::GetMesh()
{
	return GetMeshRecursive(RootNode);
}

void Model::loadModel(string path)
{
	Assimp::Importer importer;
	auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs| aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Log::GetInstance()<<Log::Level::Error<< "ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'))+"/";
	RootNode = make_shared<MeshNode>();
	processNode(scene->mRootNode, RootNode, scene);
}

void Model::processNode(aiNode* ainode, shared_ptr<MeshNode> node, const aiScene* scene)
{
	for (int i = 0; i < ainode->mNumMeshes;i++) {
		aiMesh* aimesh = scene->mMeshes[ainode->mMeshes[i]];
		node->meshes.push_back(processMesh(aimesh, scene));
	}
	for (int i = 0; i < ainode->mNumChildren; i++) {
		node->children.push_back(make_shared<MeshNode>());
		processNode(ainode->mChildren[i], node->children[i], scene);
	}
}

shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex>vertices;
	vector<unsigned int>indices;
	vector<shared_ptr<Texture>>textures;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex v;
		v.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        if (calcVertexRange) {
            if (v.Position.y > maxVertex.y && v.Position.x > maxVertex.x && v.Position.z > maxVertex.z) {
                maxVertex = v.Position;
            }
            if (v.Position.y < minVertex.y && v.Position.x < minVertex.x && v.Position.z < minVertex.z) {
                minVertex = v.Position;
            }
        }
        v.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->mTextureCoords[0]) {
			v.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			v.Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			v.BiTangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}
		else
		{
			v.TexCoords = glm::vec2(0);
			v.Tangent = glm::vec3(0);
			v.BiTangent = glm::vec3(0);
		}

		vertices.push_back(v);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	if (mesh->mMaterialIndex>0)
	{
		auto* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<shared_ptr<Texture>> ambients = loadMaterialTextures(material, aiTextureType_AMBIENT, "ambient");
		vector<shared_ptr<Texture>> diffuses = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
		vector<shared_ptr<Texture>> speculars = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
		vector<shared_ptr<Texture>> normals = loadMaterialTextures(material, aiTextureType_HEIGHT, "normal");
		textures.insert(textures.end(), ambients.begin(), ambients.end());
        textures.insert(textures.end(), diffuses.begin(), diffuses.end());
		textures.insert(textures.end(), speculars.begin(), speculars.end());
		textures.insert(textures.end(), normals.begin(), normals.end());

	}
	else
		Log::GetInstance() << Log::Level::Warning << "Loading No Material In This Mesh." << endl;
	return make_shared<Mesh>(vertices, indices, textures);
}

vector< shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName)
{
	vector< shared_ptr<Texture>>textures;
	int a = material->GetTextureCount(type);
	for (unsigned int i = 0; i < material->GetTextureCount(type);i++) {
		aiString str;
		material->GetTexture(type, i, &str);//按照默认命名规则，获取纹理文件名
		string path = directory + str.C_Str();
		bool skip = false;
		for (auto tex : textures_loaded) {
			if (path == tex->path){
				textures.push_back(tex);
				skip = true;
				break;
			}
		}
		if (!skip) {
			auto texture = make_shared<Texture>(path);//typeName =="diffuse");
			texture->type = typeName;
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}

	}
	return textures;
}
