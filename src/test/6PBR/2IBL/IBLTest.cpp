#include "IBLTest.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "MyDebug.h"
#include "Camera.h"
#include "Geometry.h"
#include "IndexBuffer.h"
#include "imgui/imgui.h"
#include <filesystem>
#include"stb_image/stb_image.h"
namespace fs = std::filesystem;

IBLTest::IBLTest()
{	
	
	//加载着色器
	{
		fs::path directory = "res/shader/6PBR/2IBL/";
		for (const auto& entry : fs::directory_iterator(directory)) {
			if (fs::is_regular_file(entry)) { // 检查是否是文件
				string filename = entry.path().filename().string();
				string path = entry.path().string();

				size_t last_dot = filename.find_last_of('.');
				if (last_dot != std::string::npos && last_dot != 0) 
					filename = filename.substr(0, last_dot);
				
				Shaders[filename] = make_shared<Shader>(path);
			}
		}
	}
	//PBR球
	{
		nrRows = 7;
		nrCols = 7;
		vector<unsigned int> indices;
		vector<float> data;
		renderSphere(data, indices, 64, 64);
		objects.push_back(make_shared<Object>());
		objects[0]->vao = make_shared<VertexArray>();
		objects[0]->vbo = make_shared<VertexBuffer>(data.data(), sizeof(float) * data.size());

		objects[0]->layout = make_shared<VertexBufferLayout>();
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(3);
		objects[0]->layout->Push<float>(2);

		ebo = make_shared<IndexBuffer>(indices.data(), indices.size());
		ebo->Bind();
		objects[0]->vao->AddBuffer(*objects[0]->vbo, *objects[0]->layout);

		//加载所有pbr材质贴图
		fs::path directory = "res/image/pbr/";
		for (const auto& entry : fs::directory_iterator(directory)) {
			if (fs::is_directory(entry)) { // 检查是否是文件
				string MaterialName = entry.path().filename().string();
				string Fullpath = entry.path().string();
				auto a =Fullpath;
				texTable[MaterialName].push_back(make_shared<Texture>(Fullpath + "/albedo.png", true));
				texTable[MaterialName].push_back(make_shared<Texture>(Fullpath + "/ao.png"));
				texTable[MaterialName].push_back(make_shared<Texture>(Fullpath + "/metallic.png"));
				texTable[MaterialName].push_back(make_shared<Texture>(Fullpath + "/normal.png"));
				texTable[MaterialName].push_back(make_shared<Texture>(Fullpath + "/roughness.png"));
				PBRTextures.push_back(MaterialName);
			}
		}
		objects[0]->pos = glm::vec3(0, 0, 0);
	}
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
	//天空盒
	{
		objects.push_back(make_shared<Object>());
		objects[1]->vao = make_shared<VertexArray>();
		objects[1]->vbo = make_shared<VertexBuffer>(CubeVertices, sizeof(CubeVertices));
		objects[1]->layout = make_shared<VertexBufferLayout>();
		objects[1]->layout->Push<float>(3);
		objects[1]->layout->Push<float>(3);
		objects[1]->layout->Push<float>(2);
		objects[1]->vao->AddBuffer(*objects[1]->vbo, *objects[1]->layout);


		HDRTextures.push_back(0);
		LoadHDR("res/image/hdr/forest_grove_4k.hdr", HDRTextures.back());
		HDRTextures.push_back(0);
		LoadHDR("res/image/hdr/newport_loft.hdr", HDRTextures.back());
		HDRTextures.push_back(0);
		LoadHDR("res/image/hdr/quarry_cloudy_4k.hdr", HDRTextures.back());
		HDRTextures.push_back(0);
		LoadHDR("res/image/hdr/qwantani_patio_4k.hdr", HDRTextures.back());
		HDRTextures.push_back(0);
		LoadHDR("res/image/hdr/rogland_clear_night_4k.hdr", HDRTextures.back());
		
	}
	//铺屏四边形 LUT
	float QuadVertices[] = {
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};
	{
		objects.push_back(make_shared<Object>());
		objects[2]->vao = make_shared<VertexArray>();
		objects[2]->vbo = make_shared<VertexBuffer>(QuadVertices, sizeof(QuadVertices));
		objects[2]->layout = make_shared<VertexBufferLayout>();
		objects[2]->layout->Push<float>(2);
		objects[2]->layout->Push<float>(2);
		objects[2]->vao->AddBuffer(*objects[2]->vbo, *objects[2]->layout);
	}
	//帧缓冲和渲染缓冲
	{
		GLCall(glGenFramebuffers(1, &CaptureFBO));
		GLCall(glGenRenderbuffers(1, &CaptureRBO));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, CaptureFBO));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, CaptureRBO));

		GLCall(GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, CaptureRBO)); )
	}
	//存储HDR的环境贴图
	{
		GLCall(glGenTextures(1, &EnvMapTexture));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, EnvMapTexture));
		for (size_t i = 0; i < 6; i++)
		{
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, EnvMapSize, EnvMapSize, 0, GL_RGB, GL_FLOAT, nullptr));
		}
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	}
	//漫反射的预计算辐照度贴图
	{
		GLCall(glGenTextures(1, &IrradianceMapTexture));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, IrradianceMapTexture));
		for (size_t i = 0; i < 6; i++)
		{
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, IrradianceMapSize, IrradianceMapSize, 0, GL_RGB, GL_FLOAT, nullptr));
		}
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	}
	//镜面反射的预滤波环境贴图
	{
		GLCall(glGenTextures(1, &PrefilterMapTexture));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, PrefilterMapTexture));
		for (size_t i = 0; i < 6; i++)
		{
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, PrefilterMapSize, PrefilterMapSize, 0, GL_RGB, GL_FLOAT, nullptr));
		}
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
	}
	//brdf LUT
	{
		GLCall(glGenTextures(1, &LUTMapTexture));
		GLCall(glBindTexture(GL_TEXTURE_2D, LUTMapTexture));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, LUTMapSize, LUTMapSize, 0, GL_RG, GL_FLOAT, 0));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	}
	lightPositions = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	lightColors = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};
}

void IBLTest::OnRender()
{	//优化
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	GLCall(glDepthFunc(GL_LEQUAL));

	//--------------预计算一次-------------//
	if (PreCalc)
	{
		PreCalc != PreCalc;
		//HDR贴图->立方体贴图
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, CaptureFBO));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, CaptureRBO));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, EnvMapSize, EnvMapSize));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glViewport(0, 0, EnvMapSize, EnvMapSize));

		objects[1]->vao->Bind();
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, HDRTextures[CurrentHDR]));
		Shaders["HDRConvertCube"]->Bind();
		Shaders["HDRConvertCube"]->SetUniform1i("equirectangularMap", 0);
		Shaders["HDRConvertCube"]->SetUniformMat4f("projection", glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f));

		glm::mat4 captureViews[] =
		{
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};
		for (size_t i = 0; i < 6; i++)
		{
			Shaders["HDRConvertCube"]->SetUniformMat4f("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, EnvMapTexture, 0);
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
			GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, EnvMapTexture);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		//卷积立方体贴图->辐射度贴图 为漫反射做预计算
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, CaptureFBO));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, CaptureRBO));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, IrradianceMapSize, IrradianceMapSize));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glViewport(0, 0, IrradianceMapSize, IrradianceMapSize));
		GLCall(glDepthFunc(GL_LEQUAL));

		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, EnvMapTexture));
		Shaders["ConvEnv2Irradiance"]->Bind();
		Shaders["ConvEnv2Irradiance"]->SetUniformMat4f("projection", glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f));
		Shaders["ConvEnv2Irradiance"]->SetUniform1i("EnvMap", 0);
		for (size_t i = 0; i < 6; i++)
		{
			Shaders["ConvEnv2Irradiance"]->SetUniformMat4f("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, IrradianceMapTexture, 0);
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
			GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}


		//卷积立方体贴图->预过滤贴图 为镜面反射做预计算
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, CaptureFBO));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, CaptureRBO));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glDepthFunc(GL_LEQUAL));
		
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, EnvMapTexture));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		Shaders["ConverEnv2PrefilterMap"]->Bind();
		Shaders["ConverEnv2PrefilterMap"]->SetUniformMat4f("projection", glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f));
		Shaders["ConverEnv2PrefilterMap"]->SetUniform1i("EnvMap", 0);
		unsigned int maxMipLevel = 5;
		for (size_t mip = 0; mip < maxMipLevel; mip++)
		{
			unsigned int MipSize = PrefilterMapSize * std::pow(0.5, mip);
			GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, MipSize, MipSize));
			GLCall(glViewport(0, 0, MipSize, MipSize));
			float roughness = mip / (maxMipLevel - 1);
			Shaders["ConverEnv2PrefilterMap"]->SetUniform1f("roughness", roughness);
			for (size_t i = 0; i < 6; i++)
			{
				Shaders["ConverEnv2PrefilterMap"]->SetUniformMat4f("view", captureViews[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, PrefilterMapTexture, mip);
				GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
				GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
				GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
			}
		}
		//LUT查找表


		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, CaptureFBO));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, CaptureRBO));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, LUTMapSize, LUTMapSize));
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, LUTMapTexture, 0);

		GLCall(glViewport(0, 0, LUTMapSize, LUTMapSize));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		objects[2]->vao->Bind();
		Shaders["brdf"]->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

	}
	//--------------绘制场景---------------//
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));

	//绘制屏幕四边形
	if (drawQuad) {
		objects[2]->vao->Bind();
		Shaders["brdf"]->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	}


	//绘制天空盒
	GLCall(glActiveTexture(GL_TEXTURE0));
	objects[1]->vao->Bind();
	switch (IBLTest::CurrentSkyBoxTexture)
	{
	case IBLTest::Env:
		Shaders["SkyBox"]->Bind();
		Shaders["SkyBox"]->SetUniformMat4f("view", m_View);
		Shaders["SkyBox"]->SetUniformMat4f("projection", m_Proj);
		Shaders["SkyBox"]->SetUniform1i("CubeMap", 0);
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, EnvMapTexture));
		break;
	case IBLTest::Irradiance:
		Shaders["SkyBox"]->Bind();
		Shaders["SkyBox"]->SetUniformMat4f("view", m_View);
		Shaders["SkyBox"]->SetUniformMat4f("projection", m_Proj);
		Shaders["SkyBox"]->SetUniform1i("CubeMap", 0);
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, IrradianceMapTexture));
		break;
	case IBLTest::Prefilter:
		Shaders["ShowPrefilter"]->Bind();
		Shaders["ShowPrefilter"]->SetUniformMat4f("view", m_View);
		Shaders["ShowPrefilter"]->SetUniformMat4f("projection", m_Proj);
		Shaders["ShowPrefilter"]->SetUniform1i("CubeMap", 0);
		Shaders["ShowPrefilter"]->SetUniform1f("MipLevel", MipLevel);
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, PrefilterMapTexture));
		break;
	
	default:
		break;
	}
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

	//绘制PBR球
	string CurrentShader = CurrentPBRTexture < texTable.size() ? "pbrTexture" : "pbr";
	Shaders[CurrentShader]->Bind();
	objects[0]->vao->Bind();
	ebo->Bind();
	
	Shaders[CurrentShader]->SetUniformMat4f("view", m_View);
	Shaders[CurrentShader]->SetUniformMat4f("projection", m_Proj);
	Shaders[CurrentShader]->SetUniform3f("ViewPos", Camera::GetInstance()->Position);
	for (int i = 0; i < lightPositions.size(); i++) {
		string str = "Lights[" + to_string(i) + "].";
		Shaders[CurrentShader]->SetUniform3f(str + "Position", lightPositions[i]);
		Shaders[CurrentShader]->SetUniform3f(str + "Color", lightColors[i]);
	}
	GLCall(glActiveTexture(GL_TEXTURE0 + 5));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, IrradianceMapTexture));
	Shaders[CurrentShader]->SetUniform1i("irradianceMap", 5);
	GLCall(glActiveTexture(GL_TEXTURE0 + 6));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, PrefilterMapTexture));
	Shaders[CurrentShader]->SetUniform1i("prefilterMap", 6);
	GLCall(glActiveTexture(GL_TEXTURE0 + 7));
	GLCall(glBindTexture(GL_TEXTURE_2D, LUTMapTexture));
	Shaders[CurrentShader]->SetUniform1i("brdfLUT", 7);
	if (CurrentPBRTexture< texTable.size())
	{
		vector<shared_ptr<Texture>> Texs = texTable[PBRTextures[CurrentPBRTexture]];
		for (size_t i = 0; i < Texs.size(); i++)
			Texs[i]->Bind(i);
		
		Shaders[CurrentShader]->SetUniform1i("albedoMap", 0);
		Shaders[CurrentShader]->SetUniform1i("aoMap", 1);
		Shaders[CurrentShader]->SetUniform1i("metallicMap", 2);
		Shaders[CurrentShader]->SetUniform1i("normalMap", 3);
		Shaders[CurrentShader]->SetUniform1i("roughnessMap", 4);
	
		float space = 3;
		for (float row = 0; row < nrRows; row++) {
			for (float col = 0; col < nrCols; col++) {
				glm::mat4 model = glm::mat4(1);
				float x = (col - nrCols / 2) * space;
				float y = (row - nrRows / 2) * space;
				model = glm::translate(model, glm::vec3(x, y, 0));
				Shaders[CurrentShader]->SetUniformMat4f("model", model);;
				GLCall(glDrawElements(GL_TRIANGLE_STRIP, ebo->GetCount(), GL_UNSIGNED_INT, 0));
			}
		}
	}
	else
	{
		Shaders[CurrentShader]->SetUniform3f("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
		float space = 3;
		for (float row = 0; row < nrRows; row++) {
			Shaders[CurrentShader]->SetUniform1f("metallic", row / nrRows);
			for (float col = 0; col < nrCols; col++) {
	
				glm::mat4 model = glm::mat4(1);
				float x = (col - nrCols / 2) * space;
				float y = (row - nrRows / 2) * space;
				model = glm::translate(model, glm::vec3(x, y, 0));
				Shaders[CurrentShader]->SetUniformMat4f("model", model);
				Shaders[CurrentShader]->SetUniform1f("roughness", glm::clamp(col / nrCols, 0.05f, 1.0f));
				Shaders[CurrentShader]->SetUniform1f("ao", 1.f);
				GLCall(glDrawElements(GL_TRIANGLE_STRIP, ebo->GetCount(), GL_UNSIGNED_INT, 0));
			}
		}
	}
	
	
}

void IBLTest::OnImGuiRender()
{

	if (ImGui::Button("Change HDR")) {
		PreCalc != PreCalc;//切换天空盒，重新生成预计算贴图
		CurrentHDR = (CurrentHDR + 1) % HDRTextures.size();
	}
	ImGui::SameLine(300);
	ImGui::Text(to_string(CurrentHDR).c_str());

	if (ImGui::Button("Show Env")) {
		CurrentSkyBoxTexture = Env;
	}ImGui::SameLine(300);
	
	if (ImGui::Button("Show Irradiance")) {
		CurrentSkyBoxTexture = Irradiance;
	}
	if (ImGui::Button("Show Prefilter")) {
		CurrentSkyBoxTexture = Prefilter;
	}ImGui::SameLine(300);
	
	ImGui::SliderFloat("MipLevel", &MipLevel, 0, 4, "% .1f");

	if (ImGui::Button("NextPBRTexture")) {
		CurrentPBRTexture = (CurrentPBRTexture + 1) % (PBRTextures.size()+1);
	}ImGui::SameLine(300);
	string Tmp = CurrentPBRTexture == PBRTextures.size() ? "None" : PBRTextures[CurrentPBRTexture];
	ImGui::Text(Tmp.c_str());
	

}
void IBLTest::LoadHDR(const string path,unsigned int &HDRTexture)
{
	stbi_set_flip_vertically_on_load(true);
	int Width, Height, nrComponents;
	float* data = stbi_loadf(path.c_str(), &Width, &Height, &nrComponents, 0);
	if (!data) {
		std::cout << "Failed to load HDR image." << std::endl;
		return;
	}
	GLCall(glGenTextures(1, &HDRTexture));
	GLCall(glBindTexture(GL_TEXTURE_2D, HDRTexture));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Width, Height, 0, GL_RGB, GL_FLOAT, data));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	stbi_image_free(data);
}