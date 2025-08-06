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
#include "CameraUtility.h"
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

//Event
#include "ApplicationEvent.h"
#include "Application.h"



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

	}

	//-----------------------------------------------------------------------------
	// Game::LoadContent()
	//		Allows you to load all content needed for your engine,
	//	    such as objects, graphics, etc.
	//-----------------------------------------------------------------------------
	bool Game::LoadContent()
	{
		CameraNodeManager::Create();
		TextureManager::Create();
		
		ShaderObjectNodeManager::Create();
		MeshNodeManager::Create();
		MaterialMan::Create();

		GameObjectManager::Create();

		this->poBufferFrame = new BufferFrame();

#pragma region Demo1

		Camera* mainCamera = new Camera();
		// ---------------------------------
		//  Camera - Setup
		// ---------------------------------
		{

			assert(mainCamera);

			CameraNodeManager::Add(Camera::Name::Default, mainCamera);

			Vec3 camPos(1, 0, 8);
			Vec3 tarVect(0, 0, 0);
			Vec3 upVect(0, 1, 0);

			mainCamera->setOrientAndPosition(upVect, tarVect, camPos);
			mainCamera->setPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);

			CameraUtility::Create(mainCamera);

			Camera* cameraA = new MovingCamera(Vec3(-4, 0, 7), Vec3(4, 0, 7));
			assert(cameraA);

			CameraNodeManager::Add(Camera::Name::CAMERA_A, cameraA);
			//-4,0,7 to 4 0 7

			camPos.set(-4, 0, 7);
			tarVect.set(-4.6, 0, 0.58);
			upVect.set(0, 1, 0);

			cameraA->setOrientAndPosition(upVect, tarVect, camPos);
			cameraA->setPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);

			Camera* cameraB = new MovingCamera(Vec3(-4, 0, 3), Vec3(4, 0, 2));
			assert(cameraB);

			//-4 0 3 to 4 0 2
			CameraNodeManager::Add(Camera::Name::CAMERA_B, cameraB);

			camPos.set(-4, 0, 3);
			tarVect.set(-4, 0, 0.5);
			upVect.set(0, 1, 0);

			cameraB->setOrientAndPosition(upVect, tarVect, camPos);
			cameraB->setPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);

			Camera* cameraC = new MovingCamera(Vec3(-3.7, 0, -1.2), Vec3(4, 0, -1.2));
			assert(cameraC);

			CameraNodeManager::Add(Camera::Name::CAMERA_C, cameraC);
			//-3.7  -1.2 to 4 -1.2
			camPos.set(-3.7, 0, -1.2);
			tarVect.set(-4.7, 0, -9);
			upVect.set(0, 1, 0);

			cameraC->setOrientAndPosition(upVect, tarVect, camPos);
			cameraC->setPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);

			Camera* cameraD = new RoatingCamera();
			assert(cameraD);

			CameraNodeManager::Add(Camera::Name::CAMERA_D, cameraD);

			camPos.set(0, 10, -1);
			tarVect.set(0, -1, 0);
			upVect.set(0, 1, 0);

			cameraD->setOrientAndPosition(upVect, tarVect, camPos);
			cameraD->setPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);

			Camera* cameraE = new Camera();
			assert(cameraD);

			//(9.466304, 0.000000, -3.843830）look at
			//(10.466292 , 0.000000, 4.156088）camera
			CameraNodeManager::Add(Camera::Name::CAMERA_E, cameraE);

			camPos.set(10.5, 0, 4);
			tarVect.set(9.5, 0, -4);
			upVect.set(0, 1, 0);

			cameraE->setOrientAndPosition(upVect, tarVect, camPos);
			cameraE->setPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);

			Camera* cameraF = new Camera();
			assert(cameraD);

			CameraNodeManager::Add(Camera::Name::CAMERA_F, cameraF);

			camPos.set(10.5, 0, 8);
			tarVect.set(9.5, 0, -4);
			upVect.set(0, 1, 0);

			cameraF->setOrientAndPosition(upVect, tarVect, camPos);
			cameraF->setPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);

			//(6.307687 , 12.775715, -0.014054）
			//(10.050784, 1.579693, 2.361037）
			Camera* cameraG = new Camera();
			assert(cameraD);

			CameraNodeManager::Add(Camera::Name::CAMERA_G, cameraG);

			camPos.set(6, 12, 0);
			tarVect.set(10, 0, 2);
			upVect.set(0, 1, 0);

			cameraG->setOrientAndPosition(upVect, tarVect, camPos);
			cameraG->setPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);

			Camera* cameraL = new RotateAroundCamera();
			assert(cameraD);

			CameraNodeManager::Add(Camera::Name::CAMERA_L, cameraL);

			camPos.set(0, 10, -1);
			tarVect.set(0, -1, 0);
			upVect.set(0, 1, 0);

			cameraL->setOrientAndPosition(upVect, tarVect, camPos);
			cameraL->setPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);
		}

#pragma region   Model + Shaders --> GraphicsObject

#pragma region CreateMesh And ShdaerObj


		// ---------------------------------
		//  MESH
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
		//  ShaderObject  ColorByVertex
		// --------------------------------

		ShaderObject* poShaderA = new ShaderObject_ColorByVertex(ShaderObject::Name::ColorByVertex);
		ShaderObjectNodeManager::Add(poShaderA);

		ShaderObject* poShaderB = new ShaderObject_FlatTexture(ShaderObject::Name::FlatTexture);
		ShaderObjectNodeManager::Add(poShaderB);

		ShaderObject* poShaderC = new ShaderObject_LightTexture(ShaderObject::Name::LightTexture);
		ShaderObjectNodeManager::Add(poShaderC);

#pragma endregion
		//ShaderObjectNodeManager::Dump();

		Material* mat = MaterialMan::Add(Material::LightTextureA, poShaderC, TextureManager::RequireTexture(TextureObject::Name::A));

		Vec3 LightColor(1, 1, 1);
		Vec3 LightPos(0, 50, 0);

		LightColor *= 1.5f;
		GraphicsObject* pGraphicsObject =
			new GraphicsObject_LightTexture(mat, meshA, LightColor, LightPos);

		GameObject* objA_1 = new GameObject(pGraphicsObject);

		objA_1->SetPos(Vec3(0, 0, 0));
		objA_1->SetScale(10.0f);

		GameObjectManager::Add("OBJA_1", objA_1);

		mat = MaterialMan::Add(Material::FlatTextureA, poShaderB, TextureManager::RequireTexture(TextureObject::Name::A));
		pGraphicsObject =new GraphicsObject(mat, meshA);

		GameObject* objA_2 = new GameObject(pGraphicsObject);

		objA_2->SetPos(Vec3(0, 0, -4));
		objA_2->SetScale(10.0f);

		GameObjectManager::Add("OBJA_2", objA_2);
		
		mat = MaterialMan::Add(Material::WireFramNoTextureA, poShaderA);
		pGraphicsObject =
			new GraphicsObject_Wireframe(mat, meshA);

		GameObject* objA_3 = new GameObject(pGraphicsObject);

		objA_3->SetPos(Vec3(0, 0, 4));
		objA_3->SetScale(10.0f);

		GameObjectManager::Add("OBJA_3", objA_3);

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
		// ------------------------------------
		// Update the camera once per frame
		// ------------------------------------
		
		CameraUtility::OrbitCamera(CameraNodeManager::GetMainCam());

		CameraNodeManager::UpdateCamera();
		GameObjectManager::Update(deltaTime);

		/*
		if ((GetKeyState('B') & 0x8000) && !present)
		{
			Trace::out("Present!\n");

			present = true;

			GameObject* objE_2 = GameObjectManager::Find("OBJE_2");
			TextureObject* newTex = TextureManager::RequireTexture(TextureObject::Name::A);

			if (objE_2)
			{
				GraphicsObject* pg1 = (GraphicsObject*)objE_2->GetGraphicsObject();
				TextureManager::Remove(pg1->GetTexture());
				pg1->SetTexture(newTex);
			}


			newTex = TextureManager::RequireTexture(TextureObject::Name::B);

			GameObject* objE_1 = GameObjectManager::Find("OBJE_1");

			if (objE_1)
			{

				GraphicsObject_LightTexture* pg2 = (GraphicsObject_LightTexture*)objE_1->GetGraphicsObject();
				TextureManager::Remove(pg2->GetTexture());
				pg2->SetTexture(newTex);
			}
		
		}
			*/

		//GameObject* d2_objB_1 = GameObjectManager::Find("D2_OBJB_1");
		//d2_objB_1->GetPos()->Print("Missing target");
	}

	//-----------------------------------------------------------------------------
	// Game::Render()
	//		This function is called once per frame
	//	    Use this for draw graphics to the screen.
	//      Only do rendering here
	//-----------------------------------------------------------------------------
	void Game::Render()
	{
		this->SetDefaultTargetMode();

		GameObjectManager::Draw();
		
		this->poBufferFrame->SetActive();
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
		delete poBufferFrame;

		TextureManager::Destroy();
		MaterialMan::Destroy();
		CameraUtility::Destroy();

		CameraNodeManager::Destroy();

		MeshNodeManager::Destroy();

		ShaderObjectNodeManager::Destroy();

		GameObjectManager::Destroy();
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

	bool Game::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		//Engine::OnWindowResizeEvent(e);

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

		this->poBufferFrame->OnResize();

		CameraNodeManager::UpdateAspectRatio(GetAspectRatio());

		return false;
	}

}




// --- End of File ---
