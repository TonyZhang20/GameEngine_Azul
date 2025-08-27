#include "SceneHierachyPanel.h"
#include "imgui.h"
#include "Components.h"
#include "Entity.h"
#include "imgui_internal.h"

namespace Azul
{
	SceneHierachyPanel::SceneHierachyPanel(Scene* scene)
		: mContext(scene), m_SelectionContext(0xffffff, 0xfff)
	{
	}

	void SceneHierachyPanel::SetContext(Scene* scene)
	{
		this->mContext = scene;
	}

	void SceneHierachyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierachy");

		auto Entities = mContext->world.Query<TagComponent>();

		for (auto id : Entities)
		{
			DrawEntityNode(id);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext.SetError();

		//右键额外菜单
		if (!m_SelectionContext && ImGui::BeginPopupContextWindow(0, 1))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				this->mContext->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionContext)
		{
			ZEntity entity(m_SelectionContext, mContext);

			DrawComponents(m_SelectionContext);
		
			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComponent");
			}

			//如果按下了按钮
			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Material") && !entity.HasComponent<MaterialComponent>())
				{
					entity.AddComponent<MaterialComponent>({
						Material::FlatTextureA,
						ShaderObject::Name::FlatTexture,
						RasterizerStateID::D3D11_FILL_SOLID,
						Material::Name::None,
					});

					entity.GetComponent<TransformComponent>().SetScale(10);
					entity.GetComponent<TransformComponent>().SetPosition({ 1, 1, 1 });

					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Mesh") && !entity.HasComponent<MeshComponent>())
				{
					entity.AddComponent<MeshComponent>({ Mesh::Name::A });
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	void SceneHierachyPanel::DrawEntityNode(zecs::EntityID entityID)
	{
		auto& tg = mContext->world.GetComponent<TagComponent>(entityID);
		
		ImGuiTreeNodeFlags flags = 
			((m_SelectionContext == entityID) ?
				ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entityID.index, flags, tg.tag);

		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entityID;
		}

		bool deleteEntity = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				deleteEntity = true;
		

			ImGui::EndPopup();
		}


		if (opened)
		{
			ImGui::TreePop();
		}

		if (deleteEntity && m_SelectionContext)
		{
			this->mContext->DestroyEntity(entityID);
			this->m_SelectionContext.SetError();
		}
	}

	void SceneHierachyPanel::DrawComponents(zecs::EntityID entity)
	{
		auto& tag = mContext->world.GetComponent<TagComponent>(entity);
		
		//Tag Component
		{	
			if (mContext->world.HasComponent<TagComponent>(entity))
		{
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, tag.tag);

			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag.SetName(buffer);
			}
		}
		}
		//TransformComponent
		{ 
		if (mContext->world.HasComponent<TransformComponent>(entity))
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), 
				ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& trans = mContext->world.GetComponent<TransformComponent>(entity);


				//ImGui::DragFloat3("Position", &trans.position[x], 0.01f);
				//ImGui::DragFloat3("Rotation", &rotation[x], 1.f, -360.f, 360.f);
				//ImGui::DragFloat3("Rotation", &trans.deltaRot[x], 1.0f);
				//ImGui::DragFloat3("Scale", &trans.scale[x], 0.01f);

				DrawVec3Control("Position", trans.position, 0.01f);
				DrawVec3Control("Rotation", trans.deltaRot, 1.f);
				DrawVec3Control("Scale", trans.scale, 0.01f, 1.0f);
				
				ImGui::TreePop();
			}

			/* Example Remove Component
			ImGui::SameLine();
			if (ImGui::Button("+", ImVec2{20, 20})) ImGui::OpenPopup("ComponentSettings");
			bool removeComponent = false;

			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (removeComponent)
				this->mContext->world.RemoveComponent<TransformComponent>(this->m_SelectionContext);
			*/
		}
		}

		//Camera Component
		{
		if (mContext->world.HasComponent<CameraComponent>(entity))
		{
			//展开菜单
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(),
				ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& camera = mContext->world.GetComponent<CameraComponent>(entity);
				auto& cam = camera.camera;

				static size_t index = 0;

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[index];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (size_t i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];

						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							index = i;
							currentProjectionTypeString = projectionTypeStrings[i];
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}	

					ImGui::EndCombo();
				}

				if (index == 0)
				{
					ImGui::DragFloat("Near", &cam.GetNear(), 0.01f);
					ImGui::DragFloat("Far", &cam.GetFar(), 0.01f);

					float fov = cam.GetFov() * MATH_180_PI;

					ImGui::DragFloat("Fov", &fov, 0.1f);

					cam.SetFov(fov * MATH_PI_180);
				}

				ImGui::TreePop();
			}
		}
		}

		//Mesh Component
		{
		if (mContext->world.HasComponent<MeshComponent>(entity))
		{
			if (ImGui::TreeNodeEx((void*)typeid(MeshComponent).hash_code(),
				ImGuiTreeNodeFlags_DefaultOpen, "Mesh"))
			{
				auto& mesh = mContext->world.GetComponent<MeshComponent>(entity);

				const char* NameStrings[] = { "CUBE", "PYRAMID", "A", "B", "C", "D", "E", "NULL_MESH", "NOT_INITIALIZED"};
				const char* currentProjectionTypeString = NameStrings[(int)mesh.meshID];

				if (ImGui::BeginCombo("CUBE", currentProjectionTypeString))
				{
					for (size_t i = 0; i < 9; i++)
					{
						bool isSelected = currentProjectionTypeString == NameStrings[i];

						if (ImGui::Selectable(NameStrings[i], isSelected))
						{
							currentProjectionTypeString = NameStrings[i];
							mesh.meshID = static_cast<Mesh::Name>(i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
			}

			ImGui::TreePop();
		}
		}

		//Material Component
		{
		if (mContext->world.HasComponent<MaterialComponent>(entity))
		{
			if (ImGui::TreeNodeEx((void*)typeid(MaterialComponent).hash_code(),
				ImGuiTreeNodeFlags_DefaultOpen, "Material"))
			{
				auto& material = mContext->world.GetComponent<MaterialComponent>(entity);

				const char* ShaderString[] = {
					"NullShader",
					"ColorByVertex",
					"FlatTexture",
					"LightTexture",
					"ConstColor",
					"Uninitialized"
				};
				const char* currentProjectionTypeString = ShaderString[(int)material.shaderID];

				if (ImGui::BeginCombo("Shader", currentProjectionTypeString))
				{
					for (size_t i = 0; i < 6; i++)
					{
						bool isSelected = currentProjectionTypeString == ShaderString[i];

						if (ImGui::Selectable(ShaderString[i], isSelected))
						{
							currentProjectionTypeString = ShaderString[i];
							material.shaderID = static_cast<ShaderObject::Name>(i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				const char* MaterialStrings[] = { "None", "DefaultMaterial", "LightTextureA", "FlatTextureA", "WireFramNoTextureA" };
				currentProjectionTypeString = MaterialStrings[(int)material.materialID];

				if (ImGui::BeginCombo("Material", currentProjectionTypeString))
				{
					for (size_t i = 0; i < 5; i++)
					{
						bool isSelected = currentProjectionTypeString == MaterialStrings[i];

						if (ImGui::Selectable(MaterialStrings[i], isSelected))
						{
							currentProjectionTypeString = MaterialStrings[i];
							material.materialID = static_cast<Material::Name>(i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				const char* RasterizerStateStrings[] = { "NOTINITIALIZE", "D3D11_FILL_SOLID", "D3D11_CULL_WIREFRAME" };
				currentProjectionTypeString = RasterizerStateStrings[(int)material.rasterizerID];

				if (ImGui::BeginCombo("Rasterizer", currentProjectionTypeString))
				{
					for (size_t i = 0; i < 3; i++)
					{
						bool isSelected = currentProjectionTypeString == RasterizerStateStrings[i];

						if (ImGui::Selectable(RasterizerStateStrings[i], isSelected))
						{
							currentProjectionTypeString = RasterizerStateStrings[i];
							material.rasterizerID = static_cast<RasterizerStateID>(i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				currentProjectionTypeString = MaterialStrings[(int)material.lightID];
				if (ImGui::BeginCombo("Light", currentProjectionTypeString))
				{
					for (size_t i = 0; i < 5; i++)
					{
						bool isSelected = currentProjectionTypeString == MaterialStrings[i];

						if (ImGui::Selectable(MaterialStrings[i], isSelected))
						{
							currentProjectionTypeString = MaterialStrings[i];
							material.lightID = static_cast<Material::Name>(i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
			}

			ImGui::TreePop();
		}
		}
	}

	void SceneHierachyPanel::DrawVec3Control(const char* label, Vec3& values, float speed, float resetValue, float columnWidth)
	{
		ImGui::PushID(label);

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label);
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = ImGui::GetFontSize() + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		//按下按钮后的yanse
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.f });

		if (ImGui::Button("X", buttonSize))
			values[x] = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values[x], speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });

		if (ImGui::Button("Y", buttonSize))
			values[y] = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values[y], speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.f });

		if (ImGui::Button("Z", buttonSize))
			values[z] = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values[z], speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
}