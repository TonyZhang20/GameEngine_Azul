
#include "Game.h"
#include "ArchetypeWorld.h"
#include "Components.h"
#include "Renderer.h"
#include "MaterialMan.h"
#include "Material.h"
#include "ShaderObjectNodeManager.h"
#include "MaterialMan.h"
#include "MeshNodeManager.h"
#include "Application.h"
#include "StateDirectXMan.h"
#include "LayerManager.h"
#include "Event.h"

namespace Azul
{
	Renderer::Renderer() :
		Layer("Basic Renderer Layer"),
		mStateRasterizerSolid(),
		mStateRasterizerWireframe(),
		mStateRenderTargetView(),
		mDepthStencilView(),
		mDepthStencilBuffer(),
		mBlendStateOff(),
		mBlendStateAlpha(),
		mViewport()
	{

	}

	Renderer::Renderer(const char* name) :
		Layer(name),
		mStateRasterizerSolid(),
		mStateRasterizerWireframe(),
		mStateRenderTargetView(),
		mDepthStencilView(),
		mDepthStencilBuffer(),
		mBlendStateOff(),
		mBlendStateAlpha(),
		mViewport()
	{

	}

	Renderer::Renderer(int order) :
		Layer(order, "Basic Renderer Layer"),
		mStateRasterizerSolid(),
		mStateRasterizerWireframe(),
		mStateRenderTargetView(),
		mDepthStencilView(),
		mDepthStencilBuffer(),
		mBlendStateOff(),
		mBlendStateAlpha(),
		mViewport()
	{
	}

	Renderer::Renderer(int order, const char* name) :
		Layer(order, name),
		mStateRasterizerSolid(),
		mStateRasterizerWireframe(),
		mStateRenderTargetView(),
		mDepthStencilView(),
		mDepthStencilBuffer(),
		mBlendStateOff(),
		mBlendStateAlpha(),
		mViewport()
	{
	}

	void Renderer::InitRenderer()
	{
		unsigned int clientWidth = Application::GetWidth();
		unsigned int clientHeight = Application::GetHeight();

		this->mStateRenderTargetView.Initialize();

		this->mDepthStencilBuffer.Initialize(clientWidth, clientHeight);
		this->mDepthStencilView.Initialize(this->mDepthStencilBuffer);

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc{ 0 };

		//update current data
		this->width = clientWidth;
		this->height = clientHeight;

		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilStateDesc.StencilEnable = FALSE;

		// Setup depth/stencil state.
		this->mStateDepthStencil.Initialize();

		// Setup blends state
		this->mBlendStateOff.Initialize(StateBlend::Mode::AlphaDisabled);

		// Disables Blending.... 
		this->mBlendStateAlpha.Initialize(StateBlend::Mode::AlphaEnabled);

		// Setup rasterizer state.
		this->mStateRasterizerSolid.Initialize(D3D11_FILL_SOLID, D3D11_CULL_FRONT);
		this->mStateRasterizerWireframe.Initialize(D3D11_FILL_WIREFRAME, D3D11_CULL_FRONT);

		// Initialize the viewport to occupy the entire client area.
		this->mViewport.Initialize(clientWidth, clientHeight);
	}

	bool Renderer::OnWindowResize(WindowResizeEvent& e)
	{
		unsigned int width = e.GetWidth();
		unsigned int height = e.GetHeight();

		//UnBind all target
		this->mStateRenderTargetView.CleanupRenderTarget();
		this->mDepthStencilView.ClearDepthStencilView();
		this->mDepthStencilBuffer.ClearDepthStencilBuffer();
		this->mStateRenderTargetView.UnBindAllRenderTarget();

		StateDirectXMan::ResizeSwapChain(width, height);
		//update current data
		this->width = width;
		this->height = height;

		//ReBind
		this->mStateRenderTargetView.Initialize();
		this->mDepthStencilBuffer.Initialize(width, height);
		this->mDepthStencilView.Initialize(this->mDepthStencilBuffer);

		//OmSet
		this->mStateRenderTargetView.Activate(this->mDepthStencilView);
		this->mViewport.ResizeViewPort(width, height);

		return false;
	}

	void Renderer::Draw(ZVector<RenderPacket>& packets, ZEntity& cam)
	{
		renderBuckets.clear();

		for (auto data : packets)
		{
			TransformComponent* transform = data.transform;
			MaterialComponent* material = data.material;
			MeshComponent* mesh = data.mesh;

			if (material->rasterizerID == RasterizerStateID::NOTINITIALIZE) continue;
			if (material->materialID == Material::Name::DefaultMaterial || material->materialID == Material::Name::None) continue;
			if (material->shaderID == ShaderObject::Name::NullShader) continue;
			if (material->shaderID == ShaderObject::Name::Uninitialized) continue;
			if (mesh->meshID == Mesh::Name::NOT_INITIALIZED) continue;
			
				if (renderBuckets.find(material->shaderID) == renderBuckets.end())
			{
				renderBuckets[material->shaderID] = ZVector<DrawData>();
			}

			renderBuckets[material->shaderID].emplace_back(DrawData{ transform, material, data.mesh });
		}

		RenderAll(cam);
	}

	void Renderer::RenderAll(ZEntity& cam)
	{
		using Transform = TransformComponent;

		auto& camTrans = cam.GetComponent<Transform>();
		auto& camComp = cam.GetComponent<CameraComponent>();

		camComp.camera.SetAspectRatio((float)width / (float)height);
		camComp.camera.updateCamera();

		Mesh::Name lastMesh = Mesh::Name::NOT_INITIALIZED;

		for (auto& bucket : renderBuckets)
		{
			if (bucket.first == ShaderObject::Name::NullShader) continue;

			ShaderObjectNode* pShader = ShaderObjectNodeManager::Find(bucket.first);

			if (!pShader) continue;
			 
			pShader->ActivateShader();
			pShader->ActivateCBV();
			
			Mat4 viewMatrix = camComp.camera.getViewMatrix(camTrans.rotation, camTrans.position);

			pShader->TransferProj(camComp.camera.getProjMatrix());
			pShader->TransferView(viewMatrix);

			for (auto& components : bucket.second)
			{
				Material* mat = MaterialMan::Find(components.material->materialID);
				MeshNode* mesh = MeshNodeManager::Find(components.mesh->meshID);
				
				Transform* trans = components.transform;

				//Set State
				if (currentState != components.material->rasterizerID)
				{
					currentState = components.material->rasterizerID;
					ActiveRasterizer();
				}

				if (lastMesh != components.mesh->meshID)
				{
					lastMesh = components.mesh->meshID;
					//Input layout
					mesh->ActivateMesh();
				}

				//world matrix
				Mat4 world = trans->GetWorldMatrix();
				pShader->TransferWorld(world);

				//material bind texture
				mat->BindTexture();

				/*
				方案1： 全部Light都传，我管你这啊哪的
				方案2： ShaderObject 处理Light，把所有光传给他，他自己挑
				目前使用方案2
				*/
				//TODO: LightManager
				if (components.material->lightID != Material::None)
				{
					//auto& light = LightManager::Find(lightID)
					Vec3 LightColor(1, 1, 1);
					Vec3 LightPos(0, 50, 0);

					pShader->TransferLight(LightPos, LightColor);
				}

				//Draw
				mesh->RenderIndexBuffer();
			}
		}
	}

	void Renderer::ActiveRasterizer()
	{
		switch (currentState)
		{
		case RasterizerStateID::D3D11_FILL_SOLID:
		{
			this->mStateRasterizerSolid.Activate();
			break;
		}
		case RasterizerStateID::D3D11_CULL_WIREFRAME:
			this->mStateRasterizerWireframe.Activate();
			break;
		default:
			break;
		}
	}

	void Renderer::SetDefaultTargetMode()
	{
		mViewport.Activate();
		mStateRenderTargetView.Activate(this->mDepthStencilView);
		this->mBlendStateOff.Activate();
		this->mStateDepthStencil.Activate();
	}

	void Renderer::ClearDepthStencilBuffer()
	{
		float clearDepth = 1.0f;
		uint8_t clearStencil = 0;

		this->mStateRenderTargetView.Clear(Application::GetWindow()->GetWindowColor());
		this->mDepthStencilView.Clear(clearDepth, clearStencil);
	}

	void Renderer::Awake()
	{
		InitRenderer();
	}

	void Renderer::Start()
	{
	}

	void Renderer::OnRender(float deltaTime)
	{
		ClearDepthStencilBuffer();

		SetDefaultTargetMode();

		Game* gameLayer = (Game*)LayerManager::Find("Engine Layer");

		//Collect Renderer Data

		if (gameLayer)
		{
			//Get Scene 现在先默认只有一个相机
			//renderer.Render(gameLayer->GetRenderPackets(), gameLayer->GetCamera());
			//Trace::out("Run Renderer\n");
			auto& cam = gameLayer->GetMainCam();
			this->Draw(gameLayer->GetRenderPacket(), cam);
		}
		
	}

	void Renderer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN_ONE(Renderer::OnWindowResize));
	}
}

