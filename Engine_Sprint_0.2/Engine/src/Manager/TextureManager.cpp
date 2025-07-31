#include "TextureManager.h"
#include "DLinkMan.h"
#include "StringThis.h"

namespace Azul
{
	TextureManager* TextureManager::posInstance = nullptr;

	void TextureManager::Create(int reserveNum, int reserveGrow)
	{
		// make sure values are ressonable 
		assert(reserveNum >= 0);
		assert(reserveGrow > 0);

		assert(posInstance == nullptr);

		// Do the initialization
		if (posInstance == nullptr)
		{
			posInstance = new TextureManager(reserveNum, reserveGrow);
		}
	}

	void TextureManager::Destroy()
	{
		TextureManager* pMan = TextureManager::privGetInstance();
		assert(pMan != nullptr);
		AZUL_UNUSED_VAR(pMan);

		delete TextureManager::posInstance;
		TextureManager::posInstance = nullptr;
	}

	TextureObject* TextureManager::Load(TextureObject::Name name)
	{
		TextureManager* pMan = TextureManager::privGetInstance();
		assert(pMan);

		TextureObject* pNode = (TextureObject*)pMan->baseAddToFront();
		assert(pNode != nullptr);

		switch (name)
		{
		case TextureObject::Name::Stone:
			pNode->Init(TextureObject::Name::Stone, L"Stone.tga");
			break;
		case TextureObject::Name::Rocks:
			pNode->Init(TextureObject::Name::Rocks, L"Rocks.tga");
			break;
		case TextureObject::Name::Duckweed:
			pNode->Init(TextureObject::Name::Duckweed, L"Duckweed.tga");
			break;
		case TextureObject::Name::A:
			pNode->Init(TextureObject::Name::A, L"A.tga");
			break;
		case TextureObject::Name::B:
			pNode->Init(TextureObject::Name::B, L"B.tga");
			break;
		case TextureObject::Name::C:
			pNode->Init(TextureObject::Name::C, L"C.tga");
			break;
		case TextureObject::Name::D:
			pNode->Init(TextureObject::Name::D, L"D.tga");
			break;
		case TextureObject::Name::E:
			pNode->Init(TextureObject::Name::E, L"E.tga");
			break;
		default:
			assert(false);
			break;
		}

		return pNode;
	}
	TextureObject* TextureManager::RequireTexture(TextureObject::Name name)
	{
		TextureManager* pMan = TextureManager::privGetInstance();
		assert(pMan != nullptr);

		TextureObject* target = pMan->Find(name);

		if (target == nullptr) //Find nothing
		{
			target = Load(name);
		}

		target->count++;
		return target;
	}

	TextureObject* TextureManager::Find(TextureObject::Name name)
	{
		TextureManager* pMan = TextureManager::privGetInstance();
		assert(pMan != nullptr);

		// Compare functions only compares two Nodes

		// So:  Use the Compare Node - as a reference
		//      use in the Compare() function
		//TextureObject* pShaderObject = pMan->poNodeCompare;
		//assert(pShaderObject);

		//pShaderObject->name = name;

		TextureObject* pData = nullptr;

		Iterator* pIt = pMan->baseGetActiveIterator();

		DLink* pNode = pIt->First();

		// Walk through the nodes
		while (!pIt->IsDone())
		{
			TextureObject* pData = (TextureObject*)pIt->Curr();

			if (pData->name == name)
			{
				return pData;
			}

			pData = nullptr;
			pNode = pIt->Next();
		}

		return pData;
	}

	void TextureManager::Remove(TextureObject* pNode)
	{
		assert(pNode != nullptr);

		TextureManager* pMan = TextureManager::privGetInstance();
		assert(pMan != nullptr);

		pNode->count--;

		if (pNode->count == 0)
		{
			Trace::out("%s Remove Texture totally\n", StringMe(pNode->name));
			pMan->baseRemove(pNode);
		}
	}

	void TextureManager::Dump()
	{
		TextureManager* pMan = TextureManager::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseDump();
	}


	TextureManager::TextureManager(int reserveNum, int reserveGrow)
		: ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
	{
		this->proFillReservedPool(reserveNum);

		// initialize derived data here
		TextureObject* pShaderObject = new TextureObject();
		assert(pShaderObject);

		this->poNodeCompare = pShaderObject;
		assert(this->poNodeCompare);

		//this->poNodeCompare->name = TextureObject::Name::Uninitialized;
	}

	TextureManager* TextureManager::privGetInstance()
	{
		assert(posInstance != nullptr);

		return posInstance;
	}

	TextureManager::~TextureManager()
	{
		delete this->poNodeCompare;
		this->poNodeCompare = nullptr;

		// iterate through the list and delete
		Iterator* pIt = this->baseGetActiveIterator();

		DLink* pNode = pIt->First();

		// Walk through the nodes
		while (!pIt->IsDone())
		{
			TextureObject* pDeleteMe = (TextureObject*)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}

		pIt = this->baseGetReserveIterator();

		pNode = pIt->First();

		// Walk through the nodes
		while (!pIt->IsDone())
		{
			TextureObject* pDeleteMe = (TextureObject*)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}

		pIt = this->baseGetReserveIterator();
	}


	DLink* TextureManager::derivedCreateNode()
	{
		DLink* pNodeBase = new TextureObject();
		assert(pNodeBase);

		return pNodeBase;
	}
}


