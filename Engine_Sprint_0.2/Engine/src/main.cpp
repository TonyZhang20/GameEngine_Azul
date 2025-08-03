//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------


//TODO: Inclucde ManageMent
#include "Application.h"
#include "WindowsWindow.h"

#include "Game.h"
#include "File.h"

using namespace Azul;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	File::SetBaseDir("");

	//Azul::Game* poGame = new Azul::Game("Engine Prototype 0.2", 1920, 1080);
	//ValueReturn = poGame->wWinMain(hInstance, prevInstance, cmdLine, cmdShow);

	//delete poGame;
	Azul::Application* app = Azul::Application::Create(hInstance, prevInstance, cmdLine, cmdShow);
	
	WindowProps props("Azul Engine", 1920, 1080);

	app->SetWindow(new WindowsWindow(hInstance, props));
	app->CreateLayers();
	app->Run();

	delete app;

	return 0;
}

// TODOLIST
// TODO: GameWindow/EditorWindow
// TODO: EditorLayer
// TODO: Gizmos
// TODO: ECS 重构
// TODO: 重构 FrameBuffer -> 类似于, Buffer.Active(), DrawCall, Buffer.Clear() -> 析构与创建
// TODO: 工具栏/菜单窗口
// TODO: 工具栏拓展功能 -> Create GameObject so on
// TODO: lua生成脚本


// --- End of File ---
