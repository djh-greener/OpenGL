#pragma once
#include<functional>
#include<iostream>
#include<vector>

#include"Object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;
class Test
{
protected:
	vector<shared_ptr<Object>>objects;
	
public:
	glm::mat4 m_Proj, m_View;
	Test():m_Proj(glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)))
	{};
	virtual ~Test() {};
	virtual void OnUpdate(float DeltaTime) {};
	virtual void OnRender() {};
	virtual void OnImGuiRender() {};
};
enum class ContentLevel {
	RootContentLevel,//根目录
	FirstContentLevel,//一级目录
	SecondContentLevel,
	ThirdContentLevel,
	FourthContentLevel,
	FifthContentLevel,
};

//目录树节点
//                         Root                                    RootContentLevel
//                       /         \                                   
//					test           game							FirstContentLevel
//              /        \           
//      1GetStart    2Light   3......							SecondContentLevel
//		/
// 1Triangle														ThirdContentLevel

struct ContentNode
{
	ContentLevel level;
	string name;
	ContentNode* father;
	vector<ContentNode*>children; //非叶节点存目录节点
	pair<string, function<Test* ()>>m_Test;//叶节点存Test实例
	ContentNode(ContentLevel level_, ContentNode* father_, string name_) {
		level = level_;
		name = name_;
		father = father_;
	}
};


//测试场景管理器，持有当前场景的指针，并管理场景切换
class TestMenu :public Test
{
	Test*& m_CurrentTest;
	ContentNode* RootNode;
	ContentNode* CurrentNode;
	//vector<pair<string, function<Test* ()>>>m_Tests;
public:
	TestMenu(Test*& CurrentTest):m_CurrentTest(CurrentTest) {
		RootNode = new ContentNode(ContentLevel::RootContentLevel, nullptr,  "Root");
		CurrentNode = RootNode;
	};
	virtual void OnImGuiRender()override;
	virtual void OnRender()override;
	template<typename T>
	void RegisterTest(const vector<string>& TestNameList)
	{
		ContentNode* NodePtr = RootNode;
		cout << "Registering Test: ";
		//沿目录树往下
		for (int i = 0; i < TestNameList.size(); i++) {
			cout << TestNameList[i] << " ";
			//横向看看当前节点下能不能找的对应ContentNode
			bool find = false;
			for (auto &it : NodePtr->children) {
				if (it->name == TestNameList[i])
				{
					find = true;
					NodePtr = it;
				}
			}
			//找不到就开新节点
			if (!find)
			{
				ContentLevel nextlevel = static_cast<ContentLevel>(static_cast<int>(NodePtr->level) + 1);
				auto it = new ContentNode(nextlevel,     NodePtr,       TestNameList[i]);
				NodePtr->children.push_back(it);
				NodePtr = it;
			}
			if (i == TestNameList.size() - 1) {
				NodePtr->m_Test = make_pair(TestNameList[i], []() {return new T; });
			}
		}
		cout << "\n";

	}
};

