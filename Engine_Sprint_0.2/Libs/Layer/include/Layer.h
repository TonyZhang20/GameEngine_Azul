#ifndef LAYER_H
#define LAYER_H

#include "LayerLabHeader.h"
#include "PCSNode.h"

namespace Azul
{
	class LAYERLIBRARY_API Layer : public PCSNode
	{
	public:
		Layer() = delete;
		Layer(const char* name);
		virtual ~Layer();

		virtual void OnAttach(){}
		virtual void OnDetach(){}
		virtual void Awake() {}
		virtual void Start() {}
		virtual void EndApp() {}
		virtual void OnUpdate(float UpdateTime){}
		virtual void OnRender(float deltaTime) {}
		virtual void OnEvent(class Event& event) {}
		virtual void OnImGuiRender() {}

		inline const char* GetName() const { return pName; };

		virtual bool Compare(PCSNode* pTargetNode) override;

		inline int GetOrder() override { return order; }

		void SetOrder(int order);
		void Set(const char* const name, const int order);

	protected:
		/// <summary>
		/// small order will render first
		/// </summary>
		int order = 0;

	};
}



#endif // !
