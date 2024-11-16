#include "Test.h"
#include"imgui/imgui.h"
#include "glad/glad.h"
#include"GLFW/glfw3.h"
#include"MyDebug.h"

//在渲染循环末尾调用
void TestMenu::OnImGuiRender()
{
	if(CurrentNode!=RootNode)
		if (ImGui::Button("<-")) {
			CurrentNode = CurrentNode->father;
			return;
		}


	if (CurrentNode->level == ContentLevel::FirstContentLevel) {
		for (auto& ChildNode : CurrentNode->children) {
			if (ImGui::Button(ChildNode->name.c_str())) {
				m_CurrentTest = ChildNode->m_Test.second();//创建新场景
				//CurrentNode = ChildNode;
			}
		}
	}
	else if (CurrentNode->level == ContentLevel::RootContentLevel )
	{
		for (auto& ChildNode : CurrentNode->children) {
			if (ImGui::Button(ChildNode->name.c_str())) {
				CurrentNode = ChildNode;
			}
		}
	}


	//for (auto& TestPair : m_Tests)
	//	//检测ImGUI的按钮是否被按下,并与已注册的Test匹配
	//	if (ImGui::Button(TestPair.first.c_str()))
	//		m_CurrentTest = TestPair.second();//创建新场景
}

void TestMenu::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}


