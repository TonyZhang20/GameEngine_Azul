#include "MainScene.h"
#include "Components.h"
#include "ZEntity.h"

namespace Azul
{
	MainScene::MainScene()
	{

	}

	MainScene::~MainScene()
	{

	}

	void MainScene::OnInit()
	{
		this->editorCamera = CreateEntity("EditorCamera");

		CameraComponent& cam = editorCamera.AddComponent<CameraComponent>();
		cam.isPriority = true;

		TransformComponent& camTrans = editorCamera.GetComponent<TransformComponent>();

		ZEntity entityA = CreateEntity("EntityA");

		MaterialComponent* zmat = &entityA.AddComponent<MaterialComponent>(MaterialComponent{
				Material::LightTextureA ,
				ShaderObject::Name::LightTexture,
				RasterizerStateID::D3D11_FILL_SOLID,
				Material::Name::LightTextureA 
			});

		MeshComponent* zmesh = &entityA.AddComponent<MeshComponent>({ Mesh::Name::A });

		TransformComponent* zTrans = &entityA.GetComponent<TransformComponent>();
		zTrans->position.set(0, 0, 0);
		zTrans->SetScale(10);

		ZEntity entityB = CreateEntity("EntityB");

		zmat = &entityB.AddComponent<MaterialComponent>(MaterialComponent{
				Material::FlatTextureA ,
				ShaderObject::Name::FlatTexture,
				RasterizerStateID::D3D11_FILL_SOLID,
				Material::Name::None 
			});

		zmesh = &entityB.AddComponent<MeshComponent>({ Mesh::Name::A });

		zTrans = &entityB.GetComponent<TransformComponent>();
		zTrans->position.set(4, 0, -6);
		zTrans->SetScale(10);

		ZEntity entityC = CreateEntity("EntityC");

		zmat = &entityC.AddComponent<MaterialComponent>(MaterialComponent
			{
				Material::WireFramNoTextureA ,
				ShaderObject::Name::ColorByVertex,
				RasterizerStateID::D3D11_CULL_WIREFRAME,
				Material::Name::None 
			});

		zmesh = &entityC.AddComponent<MeshComponent>({ Mesh::Name::A });

		zTrans = &entityC.GetComponent<TransformComponent>();
		zTrans->position.set(5, 0, -2);
		zTrans->SetScale(10);
	}

	void MainScene::Update(float deltaTime)
	{
	}

	void MainScene::OnDestroy()
	{
	}

}


