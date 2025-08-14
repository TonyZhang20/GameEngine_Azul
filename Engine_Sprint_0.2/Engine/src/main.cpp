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
#include "World.h"

using namespace Azul;

struct RenderComponent
{
	int someData;
	float someOtherData;
};

struct PhysicsComponent
{
	int someData;
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

	World world;

	EntityID entity = world.SpawnEntity(RenderComponent{ 123, 1 });
	world.AddComponent(entity, PhysicsComponent{ 456 });

	EntityID entity2 = world.SpawnEntity(RenderComponent{ 1234, 1 });
	world.AddComponent(entity2, PhysicsComponent{ 5678 });

	// 查询特定组件的Entity
	ZVector<EntityID> entityIDs = world.Query<RenderComponent, PhysicsComponent>();

	for (EntityID id : entityIDs)
	{
		RenderComponent& renderComp = world.GetComponent<RenderComponent>(id);
		PhysicsComponent& phy = world.GetComponent<PhysicsComponent>(id);

		Trace::out("Render Comp is %d, %f\n", renderComp.someData, renderComp.someOtherData);
		Trace::out("Phy Comp is %d\n", phy.someData);
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
