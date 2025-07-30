#include "Layer.h"
#include "PCSManBase.h"
#include "LayerLabHeader.h"

/// <summary>
/// TODO: Ways to Reorder the layer
/// </summary>
namespace Azul
{
	class LAYERLIBRARY_API LayerManager : public PCSManBase
	{
		enum LayerType
		{
			ImGuiLayer,
			EngineLayer
		};

	public:
		static void Add(Layer* pInNode, Layer* parent = nullptr, int order = 0);

		static void AddByOrder(Layer* pInNode);

		static PCSTreeForwardIterator GetForwardActiveIterator();
		static PCSTreeReverseIterator GetReverseActiveIterator();

		static void Attach();
		static void Detach() {};
		static void Update(float deltaTime);

		static void Create(int _reserveNum = 1, int _reserveGrow = 1);
		static void Destroy();

		LayerManager(const LayerManager&) = delete;
		LayerManager& operator = (const LayerManager&) = delete;

		virtual PCSNode* derivedCreateNode();
		static Layer* Find(const char* name);
		
		static PCSTree* GetActiveLayers();

		~LayerManager();

	protected:
		virtual PCSNode* baseAdd(PCSNode* parent) override;

	private:
		LayerManager(int _reserveNum = 3, int _reserveGrow = 1);
		static LayerManager* privGetInstance();

	private:
		static LayerManager* instance;
	};
}