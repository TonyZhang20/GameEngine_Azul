#include "MaterialMan.h"
#include "DLinkMan.h"


namespace Azul
{
	MaterialMan* MaterialMan::posInstance = nullptr;

	MaterialMan::MaterialMan(int reserveNum, int reserveGrow)
		: ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
	{
		this->poNodeCompare = new Material();
		assert(this->poNodeCompare);
	}

	MaterialMan* MaterialMan::privGetInstance()
	{
		assert(posInstance != nullptr);
		return posInstance;
	}

	MaterialMan::~MaterialMan()
	{
		delete this->poNodeCompare;
		this->poNodeCompare = nullptr;

		// iterate through the list and delete
		Iterator* pIt = this->baseGetActiveIterator();

		DLink* pNode = pIt->First();

		// Walk through the nodes
		while (!pIt->IsDone())
		{
			DLink* pDeleteMe = pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}

		pIt = this->baseGetReserveIterator();

		pNode = pIt->First();

		// Walk through the nodes
		while (!pIt->IsDone())
		{
			DLink* pDeleteMe = pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}
	}

	void MaterialMan::Destroy()
	{
		MaterialMan* pMan = MaterialMan::privGetInstance();
		assert(pMan != nullptr);
		AZUL_UNUSED_VAR(pMan);

		delete MaterialMan::posInstance;
		MaterialMan::posInstance = nullptr;
	}

	Material* MaterialMan::Add(Material::Name name, ShaderObject* shader, TextureObject* texture)
	{
		MaterialMan* pMan = MaterialMan::privGetInstance();

		assert(MaterialMan::Find(name) == nullptr);

		Material* pNode = (Material*)pMan->baseAddToFront();
		assert(pNode != nullptr);

		// Initialize the date
		// pNode->Set(name, pMesh);
		pNode->Init(shader, texture, name);

		//Return a copu
		return pNode;
	}

	Material* MaterialMan::Find(Material::Name _name)
	{
		MaterialMan* pMan = MaterialMan::privGetInstance();
		assert(pMan != nullptr);

		// Compare functions only compares two Nodes

		// So:  Use the Compare Node - as a reference
		//      use in the Compare() function
		Material* mat = pMan->poNodeCompare;
		assert(mat);

		mat->SetName(_name);

		Material* pData = (Material*)pMan->baseFind(pMan->poNodeCompare);
		return pData;
	}

	void MaterialMan::Remove(Material* pNode)
	{
		assert(pNode != nullptr);

		MaterialMan* pMan = MaterialMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseRemove(pNode);
	}

	void MaterialMan::Dump()
	{
		MaterialMan* pMan = MaterialMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseDump();
	}

	Iterator* MaterialMan::GetActiveIterator()
	{
		MaterialMan* pMan = MaterialMan::privGetInstance();
		assert(pMan != nullptr);

		return pMan->GetActiveIterator();
	}

	DLink* MaterialMan::derivedCreateNode()
	{
		return new Material();
	}

	void MaterialMan::Create(int reserveNum, int reserveGrow)
	{
		// make sure values are ressonable 
		assert(reserveNum >= 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(posInstance == nullptr);

		// Do the initialization
		if (posInstance == nullptr)
		{
			posInstance = new MaterialMan(reserveNum, reserveGrow);
		}
	}

}
