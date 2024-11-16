#include <glad/glad.h>    
#include <GLFW/glfw3.h>
#include"tools/Camera.h"

#include"1GetStart/1Triangle/TriangleTest.h"
#include"1GetStart/2Shader/ShaderTest.h"
#include"1GetStart/3Texture/TextureTest.h"
#include"1GetStart/4Transform/TransformTest.h"
#include"1GetStart/5Coordinate/CoordinateTest.h"

#include"2Light/1Color/ColorTest.h"
#include"2Light/2BaseLight/BaseLightTest.h"
#include"2Light/3Material/MaterialTest.h"
#include"2Light/4LightingMap/LightingMapTest.h"
#include"2Light/5LightCaster/LightCasterTest.h"
#include"2Light/6MultiLights/MultiLightsTest.h"

#include"3ModelLoading/ModelLoadingTest.h"

#include"4Advanced/1DeepTest/DeepTest.h"
#include "4Advanced/2StencilTest/StencilTest.h"
#include "4Advanced/3Blend/BlendTest.h"
#include "4Advanced/4FaceCulling/FaceCullingTest.h"
#include "4Advanced/5FrameBuffer/FrameBufferTest.h"
#include "4Advanced/6CubeMap/CubeMapTest.h"
#include "4Advanced/8AdvancedGLSL/AdvancedGLSLTest.h"
#include "4Advanced/9GeometryShader/GeometryShaderTest.h"
#include "4Advanced/10Instancing/InstancingTest.h"
#include "4Advanced/11AntiAliasing/AntiAliasingTest.h"

#include "5AdvancedLighting/1AdvancedLighting/AdvancedLightingTest.h"
#include "5AdvancedLighting/2GammaCorrection/GammaCorrectionTest.h"
#include "5AdvancedLighting/3Shadows/1ShadowMapping/ShadowMappingTest.h"
#include "5AdvancedLighting/3Shadows/2PointShadows/PointShadowsTest.h"
#include "5AdvancedLighting/4NormalMapping/NormalMappingTest.h"
#include "5AdvancedLighting/5ParallaxMapping/ParallaxMappingTest.h"
#include"5AdvancedLighting/6HDR/HDRTest.h"
#include"5AdvancedLighting/7Bloom/BloomTest.h"
#include"5AdvancedLighting/8DeferredShading/DeferredShadingTest.h"
#include"5AdvancedLighting/9SSAO/SSAOTest.h"


#include"6PBR/1Lighting/PBRLightingTest.h"
#include"6PBR/2IBL/IBLTest.h"


#include"imgui/imgui.h"
#include"imgui/imgui_impl_glfw_gl3.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int SCR_WIDTH = 2560;
unsigned int SCR_HEIGHT = 1440;

float lastX;
float lastY;

float deltaTime = 0;
float lastFrame = 0;

Test* currentTest = nullptr;

int main() {
	//初始化glfw，创建窗口,加入主线程
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to Creat Window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD\n";
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);//注册窗口Size回调
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	TestMenu* testMenu = new TestMenu(currentTest);
	currentTest = testMenu;
	

	testMenu->RegisterTest<TriangleTest>({"1GetStart" ,"Triangle" });
	testMenu->RegisterTest<ShaderTest>({  "1GetStart" ,"Shader" });
	testMenu->RegisterTest<TextureTest>({  "1GetStart" ,"Texture" });
	testMenu->RegisterTest<TransformTest>({  "1GetStart" ,"Transform" });
	testMenu->RegisterTest<CoordinateTest>({  "1GetStart" ,"Coordinate" });
	
	testMenu->RegisterTest<ColorTest>({  "2Light","Color" });
	testMenu->RegisterTest<BaseLightTest>({  "2Light","BaseLight" });
	testMenu->RegisterTest<MaterialTest>({  "2Light","Material" });
	testMenu->RegisterTest<LightingMapTest>({  "2Light","LightingMap" });
	testMenu->RegisterTest<LightCasterTest>({  "2Light","LightCaster" });
	testMenu->RegisterTest<MultiLightsTest>({  "2Light","MultiLights" });
	
	testMenu->RegisterTest<ModelLoadingTest>({  "3ModelLoading","ModelLoading" });

	testMenu->RegisterTest<DeepTest>({  "4Advanced", "DeepTest" });
	testMenu->RegisterTest<StencilTest>({ "4Advanced", "StencilTest" });
	testMenu->RegisterTest<BlendTest>({ "4Advanced", "Blend" });
	testMenu->RegisterTest<FaceCullingTest>({ "4Advanced", "FaceCulling" });
	testMenu->RegisterTest<FrameBufferTest>({ "4Advanced", "FrameBuffer" });
	testMenu->RegisterTest<CubeMapTest>({ "4Advanced", "CubeMap" });
	testMenu->RegisterTest<AdvancedGLSLTest>({ "4Advanced", "AdvancedGLSL" });
	testMenu->RegisterTest<GeometryShaderTest>({ "4Advanced", "GeometryShader" });
	testMenu->RegisterTest<InstancingTest>({ "4Advanced", "Instancing" });
	testMenu->RegisterTest<AntiAliasingTest>({ "4Advanced", "AntiAliasing" });

	testMenu->RegisterTest<AdvancedLightingTest>({ "5AdvancedLighting","AdvancedLighting" });
	testMenu->RegisterTest<GammaCorrectionTest>({ "5AdvancedLighting","GammaCorrection" });
	testMenu->RegisterTest<ShadowMappingTest>({ "5AdvancedLighting","Shadows : ShadowMapping" });
	testMenu->RegisterTest<PointShadowsTest>({ "5AdvancedLighting","Shadows : PointShadows" });
	testMenu->RegisterTest<NormalMappingTest>({ "5AdvancedLighting","NormalMapping" });
	testMenu->RegisterTest<ParallaxMappingTest>({ "5AdvancedLighting","ParallaxMapping" });
	testMenu->RegisterTest<HDRTest>({ "5AdvancedLighting","HDR" });
	testMenu->RegisterTest<BloomTest>({ "5AdvancedLighting","Bloom" });
	testMenu->RegisterTest<DeferredShadingTest>({ "5AdvancedLighting","DeferredShading" });
	testMenu->RegisterTest<SSAOTest>({ "5AdvancedLighting","SSAO" });

	testMenu->RegisterTest<PBRLightingTest>({ "6PBR","PBRLighting" });
	testMenu->RegisterTest<IBLTest>({ "6PBR","IBL" });
	
	
	while (!glfwWindowShouldClose(window))
	{
		if (auto it = dynamic_cast<Test*>(currentTest)) {
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			Camera::GetInstance()->ProcessKeyboard(window, deltaTime);
			it->m_View = Camera::GetInstance()->GetViewMatrix();
			it->m_Proj = Camera::GetInstance()->GetProjMatrix();
		}
		ImGui_ImplGlfwGL3_NewFrame();
		if (currentTest) {
			currentTest->OnUpdate(deltaTime);
			currentTest->OnRender();
			ImGui::Begin("Test");
			if (currentTest != testMenu && ImGui::Button("<-")) {
				delete currentTest;
				currentTest = testMenu;
			}
			currentTest->OnImGuiRender();
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();//检查事件，调用回调函数
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}


void  FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
	if (auto it = dynamic_cast<Test*>(currentTest))
		it->m_Proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
	lastX = xpos;
	lastY = ypos;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		Camera::GetInstance()->ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//不知道为什么这个回调函数无效
	Camera::GetInstance()->ProcessScroll(yoffset);
}

