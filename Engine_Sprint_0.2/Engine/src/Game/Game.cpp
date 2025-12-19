//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include <d3d11.h>
#include <d3dcompiler.h>

#include "Game.h"
#include "Engine.h"

//Camera
#include "Camera.h"
#include "MovingCamera.h"
#include "RotatingCamera.h"
#include "RotateAroundCamera.h"

//Tool
#include "MathEngine.h"
#include "Colors.h"
#include "Input.h"

//Shader
#include "MeshHeaders.h"
#include "TextureObject.h"
#include "ShaderObjectHeaders.h"
#include "GraphicsObjectHeaders.h"

//GameObject
#include "GameObject_RotateAround.h"
#include "GameObject.h"

#include "Material.h"
#include "MaterialMan.h"

//Manager
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "CameraNodeManager.h"
#include "StateDirectXMan.h"
#include "SceneManager.h"
#include "MainScene.h"

//Event
#include "ApplicationEvent.h"
#include "Application.h"
#include "CameraUtility.h"



namespace Azul
{
	Game* poGame;
	static bool present = false;
	// Demo parameters

	Game::Game()
		: Engine()
	{
		poGame = this;
	}

	Game::~Game()
	{
		OnDetach();
	}

	//-----------------------------------------------------------------------------
	// Game::LoadContent()
	//		Allows you to load all content needed for your engine,
	//	    such as objects, graphics, etc.
	//-----------------------------------------------------------------------------
	bool Game::LoadContent()
	{
		TextureManager::Create();
		SceneManager::Create();
		ShaderObjectNodeManager::Create();
		MeshNodeManager::Create();
		MaterialMan::Create();


		this->mainScene = new MainScene();

		SceneManager::AddScene(mainScene);
		SceneManager::Init();

		this->camera = mainScene->GetMainCamera();


#pragma region Demo1

		CameraComponent& cam = camera.GetComponent<CameraComponent>();
		TransformComponent& camTrans = camera.GetComponent<TransformComponent>();

		///
		/// Entity Camera
		///  
		{
			Vec3 camPos(1, 0, 4);
			Vec3 tarVect(0, 0, 5);
			Vec3 upVect(0, 1, 0);

			cam.camera.SetCameraEntity(&camera);

			cam.camera.setPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);
			camTrans.position.set(camPos);
			camTrans.SetForward(tarVect, &upVect);
		}

		//mainCamera->setOrientAndPosition(upVect, tarVect, camPos);
		//mainCamera->setPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);

#pragma region   Model + Shaders --> GraphicsObject

#pragma region CreateMesh And ShdaerObj

		// ---------------------------------
		//  MESH -> Load Mesh
		// ---------------------------------
		Mesh* poModelA = new MeshCube();
		Mesh* poModelB = new MeshPyramid();
		Mesh* meshA = new MeshReadFile("A");
		Mesh* meshB = new MeshReadFile("B");
		Mesh* meshC = new MeshReadFile("C");
		Mesh* meshD = new MeshReadFile("D");
		Mesh* meshE = new MeshReadFile("E");

		MeshNodeManager::Add(Mesh::Name::CUBE, poModelA);
		MeshNodeManager::Add(Mesh::Name::PYRAMID, poModelB);
		MeshNodeManager::Add(Mesh::Name::A, meshA);
		MeshNodeManager::Add(Mesh::Name::B, meshB);
		MeshNodeManager::Add(Mesh::Name::C, meshC);
		MeshNodeManager::Add(Mesh::Name::D, meshD);
		MeshNodeManager::Add(Mesh::Name::E, meshE);

		// --------------------------------
		//  ShaderObject  ColorByVertex -> Load Shader
		// --------------------------------
		ShaderObject* poShaderA = new ShaderObject_ColorByVertex(ShaderObject::Name::ColorByVertex);
		ShaderObjectNodeManager::Add(poShaderA);

		ShaderObject* poShaderB = new ShaderObject_FlatTexture(ShaderObject::Name::FlatTexture);
		ShaderObjectNodeManager::Add(poShaderB);

		ShaderObject* poShaderC = new ShaderObject_LightTexture(ShaderObject::Name::LightTexture);
		ShaderObjectNodeManager::Add(poShaderC);
#pragma endregion

		//ShaderObjectNodeManager::Dump();

		MaterialMan::Add(Material::LightTextureA, poShaderC, TextureManager::RequireTexture(TextureObject::Name::A));
		MaterialMan::Add(Material::FlatTextureA, poShaderB, TextureManager::RequireTexture(TextureObject::Name::A));
		MaterialMan::Add(Material::WireFramNoTextureA, poShaderA, nullptr);

#pragma endregion

#pragma endregion
		return true;
	}

	//-----------------------------------------------------------------------------
	// Game::Update()
	//      Called once per frame, update data, tranformations, etc
	//      Use this function to control process order
	//      Input, AI, Physics, Animation, and Graphics
	//-----------------------------------------------------------------------------

	void Game::Update(float deltaTime)
	{
		this->mainScene->Update(deltaTime);

		CollectRenderPackets();

		this->camera.GetComponent<CameraComponent>().camera.updateCamera(deltaTime);
	}

	//-----------------------------------------------------------------------------
	// Game::Render()
	//		This function is called once per frame
	//	    Use this for draw graphics to the screen.
	//      Only do rendering here
	//-----------------------------------------------------------------------------
	void Game::Render()
	{
		//Render Layout will handle the Render part
		//Render is in Applicaiton
	}

	//-----------------------------------------------------------------------------
	// Game::UnLoadContent()
	//       unload content (resources loaded above)
	//       unload all content that was loaded before the Engine Loop started
	//-----------------------------------------------------------------------------
	void Game::UnloadContent()
	{
		//auto instance = GameObjectManager::instance;
		//AZUL_UNUSED_VAR(instance);

		SceneManager::Destroy();
		TextureManager::Destroy();
		MaterialMan::Destroy();
		MeshNodeManager::Destroy();
		ShaderObjectNodeManager::Destroy();
	}

	//------------------------------------------------------------------
	// Game::ClearBufferFunc()
	// Allows user to change the way the clear buffer function works
	// 
	// Clear the color and depth buffers.
	//------------------------------------------------------------------
	void Game::ClearDepthStencilBuffer()
	{

		float clearDepth = 1.0f;
		uint8_t clearStencil = 0;

		this->mStateRenderTargetView.Clear(Application::GetWindow()->GetWindowColor());
		this->mDepthStencilView.Clear(clearDepth, clearStencil);
	}

	void Game::OnDetach()
	{
		this->UnloadContent();
		this->Cleanup();
	}

	bool Game::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		//Engine::OnWindowResizeEvent(e);
		return false;

		UINT width, height;
		width = e.GetWidth();
		height = e.GetHeight();

		//UnBind all target
		this->mStateRenderTargetView.CleanupRenderTarget();
		this->mDepthStencilView.ClearDepthStencilView();
		this->mDepthStencilBuffer.ClearDepthStencilBuffer();
		this->mStateRenderTargetView.UnBindAllRenderTarget();

		StateDirectXMan::ResizeSwapChain(width, height);

		//ReBind
		this->mStateRenderTargetView.Initialize();
		this->mDepthStencilBuffer.Initialize(width, height);
		this->mDepthStencilView.Initialize(this->mDepthStencilBuffer);

		//OmSet
		this->mStateRenderTargetView.Activate(this->mDepthStencilView);
		this->mViewport.ResizeViewPort(width, height);

		CameraNodeManager::UpdateAspectRatio(GetAspectRatio());

		return false;
	}

	bool Game::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		Input::SetMouse(e.GetX(), e.GetY());
		return false;
	}

	ZEntity& Game::GetMainCam()
	{
		return this->camera;
	}

}




// --- End of File ---
