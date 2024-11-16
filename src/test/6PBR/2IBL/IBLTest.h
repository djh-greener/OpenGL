#pragma once
#include "Test.h"
class Shader;
class IndexBuffer;

class IBLTest :
    public Test
{
	unordered_map<string, shared_ptr<Shader>>Shaders;
	shared_ptr<IndexBuffer>ebo;

	vector<glm::vec3> lightPositions, lightColors;

	bool PreCalc = true;
	unsigned int CaptureFBO;
	unsigned int CaptureRBO;
	unsigned int EnvMapTexture, IrradianceMapTexture, PrefilterMapTexture,LUTMapTexture;
	int EnvMapSize = 512, IrradianceMapSize = 32, PrefilterMapSize = 128,LUTMapSize =512;

	vector<unsigned int >HDRTextures;
	int CurrentHDR = 0;

	unordered_map<string, vector<shared_ptr<Texture>>> texTable;
	vector<string>PBRTextures;
	int CurrentPBRTexture = 0;

	float nrRows, nrCols;//PBR«Ú’Û¡– ˝
	enum SkyBoxTexture {
		Env,
		Irradiance,
		Prefilter,
	}CurrentSkyBoxTexture;
	float MipLevel = 0;
	bool drawQuad = false;

private:
	void LoadHDR(const string path, unsigned int& HDRTexture);

public:
	IBLTest();
	virtual void OnRender()override;
	virtual void OnImGuiRender()override;
};

