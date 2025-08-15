//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------


//TODO: Inclucde ManageMent
#include "Application.h"
#include "WindowsWindow.h"

#include "Game.h"
#include "File.h"

#include "ZVector.hpp"
#include "MemEngine.h"

#include "Entity.h"
#include "ArchetypeWorld.h"

using namespace Azul;

struct Physics
{
	float force;
	float x, y, z;
};

struct Shader
{
	int vs, fs;
};

struct Transform
{
	float x, y, z;
};

using namespace zecs;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	//File::SetBaseDir("");

	//Azul::Application* app = Azul::Application::Create(hInstance, prevInstance, cmdLine, cmdShow);
	//
	//WindowProps props("Azul Engine", 1920, 1080);

	//app->SetWindow(new WindowsWindow(hInstance, props));
	//app->CreateLayers();
	//app->Run();

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

	ArchetypeWorld world;

	for (int i = 0; i < 100; i++)
	{
		auto entity = world.SpawnEntity<Transform>(Transform{ 1.0f * i, 2.0f * i, 3.0f * i });
		if (i % 2)
		{
			world.AddComponent<Shader>(entity, Shader{ i, 2 * i });
		}
		if (i % 3)
		{
			world.AddComponent<Physics>(entity, Physics{ 0.5f * i, 1.0f * i, 2.0f * i, 3.0f * i });
		}
		if (i % 6)
		{
			world.DestroyEntity(entity);
		}
	}

	for (auto e : world.Query<Shader, Physics>())
	{
		world.GetComponent<Shader>(e).vs++;
		world.GetComponent<Physics>(e).force++;
	}

	Mem::Destroy();

	return 0;
}

// TODOLIST
// TODO: ECS 重构
// TODO: 学反射
// TODO: GameWindow/EditorWindow
// TODO: EditorLayer
// TODO: Gizmos
// TODO: 重构 FrameBuffer -> 类似于, Buffer.Active(), DrawCall, Buffer.Clear() -> 析构与创建
// TODO: 工具栏/菜单窗口
// TODO: 工具栏拓展功能 -> Create GameObject so on
// TODO: lua生成脚本


// --- End of File ---
