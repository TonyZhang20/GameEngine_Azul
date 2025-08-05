//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------


//TODO: Inclucde ManageMent
#include "Application.h"
#include "WindowsWindow.h"

#include "Game.h"
#include "File.h"

#include "MemEngine.h"


using namespace Azul;
/*
struct TestClass
{
	TestClass() = default;
	TestClass(int i) { e = i; };

	~TestClass() 
	{ 
		//Trace::out("Decontruct\n"); 
	};

	void Print()
	{
		Trace::out("%d\n", e);
	}

	bool operator==(const TestClass& other) const
	{
		return false;
	}

	int e = 1;
	double a;
	float b;
	char c[32];
	TestClass* p = nullptr;
};*/

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	Mem::Create();

	File::SetBaseDir("");

	Azul::Application* app = Azul::Application::Create(hInstance, prevInstance, cmdLine, cmdShow);
	
	WindowProps props("Azul Engine", 1920, 1080);

	app->SetWindow(new WindowsWindow(hInstance, props));
	app->CreateLayers();
	app->Run();

	delete app;
	/*
	AnimTimer timer;

	AnimTime micro(AnimTime::Duration::ONE_MICROSECOND);
	AnimTime time;

	
	int size = 30000;


	timer.Tic();
	ZHashMap<int, TestClass> myMap;
	 
	for (int i = 0; i < size; i++)
	{
		myMap[i] = TestClass();
	}

	for (int i = 0; i < size; i++)
	{
		assert(myMap[i].e == 1);
	}

	time = timer.Toc();


	int t = AnimTime::Quotient(time, micro);
	float actualTime = (float)t / 1000.f;
	Trace::out("Mine Map is %f\n", actualTime);

	timer.Tic();

	std::unordered_map<int, TestClass> stlMap;


	for (int i = 0; i < size; i++)
	{
		stlMap[i] = TestClass();
	}

	for (int i = 0; i < size; i++)
	{
		assert(stlMap[i].e == 1);
	}

	time = timer.Toc();

	t = AnimTime::Quotient(time, micro);
	actualTime = (float)t / 1000.f;
	Trace::out("STL Map is %f\n", actualTime);
	*/


	Mem::Destroy();



	return 0;
}

// TODOLIST
// TODO: ECS 重构
// TODO: GameWindow/EditorWindow
// TODO: EditorLayer
// TODO: Gizmos
// TODO: 重构 FrameBuffer -> 类似于, Buffer.Active(), DrawCall, Buffer.Clear() -> 析构与创建
// TODO: 工具栏/菜单窗口
// TODO: 工具栏拓展功能 -> Create GameObject so on
// TODO: lua生成脚本


// --- End of File ---
