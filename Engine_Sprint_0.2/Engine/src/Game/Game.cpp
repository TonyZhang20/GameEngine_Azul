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

//Shader
#include "MeshHeaders.h"
#include "TextureObject.h"
#include "ShaderObjectHeaders.h"
#include "GraphicsObjectHeaders.h"

//GameObject
#include "GameObject_RotateAround.h"
#include "GameObject.h"

//Manager
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "CameraNodeManager.h"

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
		
		GameObjectManager::Create();

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

#pragma region OBJA

		Vec3 LightColor(1, 1, 1);
		Vec3 LightPos(0, 50, 0);

		LightColor *= 1.5f;
		GraphicsObject* pGraphicsObject =
			new GraphicsObject_LightTexture(meshA, poShaderC, TextureObject::Name::A, LightColor, LightPos);

		GameObject* objA_1 = new GameObject(pGraphicsObject);

		objA_1->SetPos(Vec3(0, 0, 0));
		objA_1->SetScale(10.0f);

		GameObjectManager::Add("OBJA_1", objA_1);

		pGraphicsObject =
			new GraphicsObject_FlatTexture(meshA, poShaderB, TextureObject::Name::A);

		GameObject* objA_2 = new GameObject(pGraphicsObject);

		objA_2->SetPos(Vec3(0, 0, -4));
		objA_2->SetScale(10.0f);

		GameObjectManager::Add("OBJA_2", objA_2);

		pGraphicsObject =
			new GraphicsObject_Wireframe(meshA, poShaderA);

		GameObject* objA_3 = new GameObject(pGraphicsObject);

		objA_3->SetPos(Vec3(0, 0, 4));
		objA_3->SetScale(10.0f);

		GameObjectManager::Add("OBJA_3", objA_3);

#pragma endregion

#pragma region OBJB

		pGraphicsObject =
			new GraphicsObject_LightTexture(meshB, poShaderC, TextureObject::Name::B, LightColor, LightPos);

		GameObject* objB_1 = new GameObject(pGraphicsObject);

		objB_1->SetPos(Vec3(2, 0, 0));
		objB_1->SetScale(10.0f);

		GameObjectManager::Add("OBJB_1", objB_1);

		pGraphicsObject =
			new GraphicsObject_FlatTexture(meshB, poShaderB, TextureObject::Name::B);

		GameObject* objB_2 = new GameObject(pGraphicsObject);

		objB_2->SetPos(Vec3(2, 0, -4));
		objB_2->SetScale(10.0f);

		GameObjectManager::Add("OBJB_2", objB_2);

		pGraphicsObject =
			new GraphicsObject_Wireframe(meshB, poShaderA);

		GameObject* objB_3 = new GameObject(pGraphicsObject);

		objB_3->SetPos(Vec3(2, 0, 4));
		objB_3->SetScale(10.0f);

		GameObjectManager::Add("OBJB_3", objB_3);

#pragma endregion

#pragma region OBJC

		pGraphicsObject =
			new GraphicsObject_LightTexture(meshC, poShaderC, TextureObject::Name::C, LightColor, LightPos);

		GameObject* objC_1 = new GameObject(pGraphicsObject);

		objC_1->SetPos(Vec3(-2, 0, 0));
		objC_1->SetScale(10.0f);

		GameObjectManager::Add("OBJC_1", objC_1);

		pGraphicsObject =
			new GraphicsObject_FlatTexture(meshC, poShaderB, TextureObject::Name::C);

		GameObject* objC_2 = new GameObject(pGraphicsObject);

		objC_2->SetPos(Vec3(-2, 0, -4));
		objC_2->SetScale(10.0f);

		GameObjectManager::Add("OBJC_2", objC_2);

		pGraphicsObject =
			new GraphicsObject_Wireframe(meshC, poShaderA);

		GameObject* objC_3 = new GameObject(pGraphicsObject);

		objC_3->SetPos(Vec3(-2, 0, 4));
		objC_3->SetScale(10.0f);

		GameObjectManager::Add("OBJC_3", objC_3);

#pragma endregion

#pragma region OBJD

		pGraphicsObject =
			new GraphicsObject_LightTexture(meshD, poShaderC, TextureObject::Name::D, LightColor, LightPos);

		GameObject* objD_1 = new GameObject(pGraphicsObject);

		objD_1->SetPos(Vec3(-4, 0, 0));
		objD_1->SetScale(10.0f);

		GameObjectManager::Add("OBJD_1", objD_1);

		pGraphicsObject =
			new GraphicsObject_FlatTexture(meshD, poShaderB, TextureObject::Name::D);

		GameObject* objD_2 = new GameObject(pGraphicsObject);

		objD_2->SetPos(Vec3(-4, 0, -4));
		objD_2->SetScale(10.0f);

		GameObjectManager::Add("OBJD_2", objD_2);

		pGraphicsObject =
			new GraphicsObject_Wireframe(meshD, poShaderA);

		GameObject* objD_3 = new GameObject(pGraphicsObject);

		objD_3->SetPos(Vec3(-4, 0, 4));
		objD_3->SetScale(10.0f);

		GameObjectManager::Add("OBJD_3", objD_3);

#pragma endregion

#pragma region OBJE

		pGraphicsObject =
			new GraphicsObject_LightTexture(meshE, poShaderC, TextureObject::Name::E, LightColor, LightPos);

		GameObject* objE_1 = new GameObject(pGraphicsObject);

		objE_1->SetPos(Vec3(4, 0, 0));
		objE_1->SetScale(10.0f);

		GameObjectManager::Add("OBJE_1", objE_1);

		pGraphicsObject =
			new GraphicsObject_FlatTexture(meshE, poShaderB, TextureObject::Name::E);

		GameObject* objE_2 = new GameObject(pGraphicsObject);

		objE_2->SetPos(Vec3(4, 0, -4));
		objE_2->SetScale(10.0f);

		GameObjectManager::Add("OBJE_2", objE_2);

		pGraphicsObject =
			new GraphicsObject_Wireframe(meshE, poShaderA);

		GameObject* objE_3 = new GameObject(pGraphicsObject);

		objE_3->SetPos(Vec3(4, 0, 4));
		objE_3->SetScale(10.0f);

		GameObjectManager::Add("OBJE_3", objE_3);

#pragma endregion


#pragma endregion

#pragma endregion Demo 1

#pragma region Demo2

		pGraphicsObject =
			new GraphicsObject_LightTexture(meshA, poShaderC, TextureObject::Name::A, LightColor, LightPos);

		GameObject* d2_objA_1 = new GameObject(pGraphicsObject);
		//(9.466304, 0.000000, -3.843830）look at
		//(10.466292 , 0.000000, 4.156088）camera
		d2_objA_1->SetPos(Vec3(10, 0, 0));
		d2_objA_1->SetScale(10.0f);

		GameObjectManager::Add("D2_OBJA_1", d2_objA_1);

		pGraphicsObject =
			new GraphicsObject_LightTexture(meshA, poShaderC, TextureObject::Name::A, LightColor, LightPos);

		GameObject_RotateAround* d2_objA_2 = new GameObject_RotateAround(pGraphicsObject);
		d2_objA_2->SetTarget(d2_objA_1);
		d2_objA_2->SetScale(10);

		GameObjectManager::Add("D2_OBJA_2", d2_objA_2);

		pGraphicsObject =
			new GraphicsObject_LightTexture(meshB, poShaderC, TextureObject::Name::B, LightColor, LightPos);

		GameObject_RotateAround* d2_objB_1 = new GameObject_RotateAround(pGraphicsObject);

		d2_objB_1->SetTarget(d2_objA_2);
		d2_objB_1->SetDir(Rot1::X);
		d2_objB_1->SetScale(10);
		d2_objB_1->SetLightColor(Colors::Brown, Colors::White);

		GameObjectManager::Add("D2_OBJB_1", d2_objB_1);

		pGraphicsObject =
			new GraphicsObject_LightTexture(meshC, poShaderC, TextureObject::Name::C, LightColor, LightPos);

		GameObject_RotateAround* d2_objC_1 = new GameObject_RotateAround(pGraphicsObject);

		d2_objC_1->SetTarget(d2_objB_1);
		d2_objC_1->SetDir(Rot1::Y);
		d2_objC_1->SetScale(10);
		d2_objC_1->SetLightColor(Colors::Green, Colors::Blue);

		GameObjectManager::Add("D2_OBJC_1", d2_objC_1);

		RotateAroundCamera* cameraL = (RotateAroundCamera * )CameraNodeManager::Find(Camera::Name::CAMERA_L)->GetCamera();
		cameraL->target = d2_objA_1;

#pragma endregion Demo2


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

		if ((GetKeyState('B') & 0x8000) && !present)
		{
			Trace::out("Present!\n");

			present = true;

			GameObject* objE_2 = GameObjectManager::Find("OBJE_2");
			TextureObject* newTex = TextureManager::RequireTexture(TextureObject::Name::A);

			if (objE_2)
			{
				GraphicsObject_FlatTexture* pg1 = (GraphicsObject_FlatTexture*)objE_2->GetGraphicsObject();
				TextureManager::Remove(pg1->pTex);
				pg1->pTex = newTex;
			}


			newTex = TextureManager::RequireTexture(TextureObject::Name::B);

			GameObject* objE_1 = GameObjectManager::Find("OBJE_1");

			if (objE_1)
			{

				GraphicsObject_LightTexture* pg2 = (GraphicsObject_LightTexture*)objE_1->GetGraphicsObject();
				TextureManager::Remove(pg2->pTex);
				pg2->pTex = newTex;
			}

		}

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
		TextureManager::Destroy();

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
		Engine::OnWindowResizeEvent(e);
		CameraNodeManager::UpdateAspectRatio(GetAspectRatio());

		return false;
	}

}




// --- End of File ---
