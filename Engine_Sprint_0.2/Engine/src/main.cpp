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

//TODO LIST :
// Graphic Object rework //alomst
// Material //done
// Material Manager : Do nothing just delete Material after application finished ---- Done
// RenderQueue Manager : For better performence //done
// Transform

// --- End of File ---
