#include "Test.h"
#include"imgui/imgui.h"
#include "glad/glad.h"
#include"GLFW/glfw3.h"
#include"MyDebug.h"

//����Ⱦѭ��ĩβ����
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
				m_CurrentTest = ChildNode->m_Test.second();//�����³���
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
	//	//���ImGUI�İ�ť�Ƿ񱻰���,������ע���Testƥ��
	//	if (ImGui::Button(TestPair.first.c_str()))
	//		m_CurrentTest = TestPair.second();//�����³���
}

void TestMenu::OnRender()
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}


