#include "CameraNodeManager.h"
#include "CameraNULL.h"
#include "DLinkMan.h"
#include "CameraUtility.h"

namespace Azul
{
	CameraNodeManager* CameraNodeManager::posInstance = nullptr;

	CameraNodeManager::CameraNodeManager(int reserveNum, int reserveGrow)
		: ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
	{
		// Preload the reserve
		this->proFillReservedPool(reserveNum);

		// initialize derived data here
		Camera* pCam = new CameraNULL();
		assert(pCam);
		this->poNodeCompare = new CameraNode();
		assert(this->poNodeCompare);
		this->poNodeCompare->Set(Camera::Name::NULL_CAMERA, pCam);

		this->poMainCam = new CameraNode();
		this->poMainCam->Set(Camera::Name::MAIN_CAMERA, nullptr);
	}

	CameraNodeManager::~CameraNodeManager()
	{
		delete this->poNodeCompare;
		this->poNodeCompare = nullptr;

		this->poMainCam->Set(Camera::Name::NULL_CAMERA, nullptr);
		delete this->poMainCam;
		
		this->poMainCam = nullptr;

		// iterate through the list and delete
		Iterator* pIt = this->baseGetActiveIterator();

		DLink* pNode = pIt->First();

		// Walk through the nodes
		while (!pIt->IsDone())
		{
			CameraNode* pDeleteMe = (CameraNode*)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}

		pIt = this->baseGetReserveIterator();

		pNode = pIt->First();

		// Walk through the nodes
		while (!pIt->IsDone())
		{
			CameraNode* pDeleteMe = (CameraNode*)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}
	}

	void CameraNodeManager::Create(int reserveNum, int reserveGrow)
	{
		// make sure values are ressonable 
		assert(reserveNum >= 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(posInstance == nullptr);

		// Do the initialization
		if (posInstance == nullptr)
		{
			posInstance = new CameraNodeManager(reserveNum, reserveGrow);
		}
	}


	void CameraNodeManager::Destroy()
	{
		CameraNodeManager* pMan = CameraNodeManager::privGetInstance();
		assert(pMan != nullptr);
		AZUL_UNUSED_VAR(pMan);

		delete CameraNodeManager::posInstance;
		CameraNodeManager::posInstance = nullptr;
	}

	CameraNode* CameraNodeManager::Add(Camera::Name name, Camera* pCam)
	{
		CameraNodeManager* pMan = CameraNodeManager::privGetInstance();

		assert(pCam);

		CameraNode* pNode = (CameraNode*)pMan->baseAddToFront();
		assert(pNode != nullptr);

		// Initialize the date
		pNode->Set(name, pCam);

		if (GetMainCam() == nullptr)
		{
			SetMainCam(pCam);
		}

		return pNode;
	}

	CameraNode* CameraNodeManager::Find(Camera::Name _name)
	{
		CameraNodeManager* pMan = CameraNodeManager::privGetInstance();
		assert(pMan != nullptr);

		// Compare functions only compares two Nodes

		// So:  Use the Compare Node - as a reference
		//      use in the Compare() function
		Camera* pMesh = pMan->poNodeCompare->GetCamera();
		assert(pMesh);

		pMesh->name = _name;

		CameraNode* pData = (CameraNode*)pMan->baseFind(pMan->poNodeCompare);
		return pData;
	}

	Camera* CameraNodeManager::GetMainCam()
	{
		CameraNodeManager* pMan = CameraNodeManager::privGetInstance();
		return pMan->poMainCam->GetCamera();
	}

	void CameraNodeManager::SetMainCam(Camera* cam)
	{
		assert(cam);

		CameraNodeManager* pMan = CameraNodeManager::privGetInstance();
		pMan->poMainCam->Set(cam);
	}

	void CameraNodeManager::Remove(CameraNode* pNode)
	{
		assert(pNode != nullptr);

		CameraNodeManager* pMan = CameraNodeManager::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseRemove(pNode);
	}

	void CameraNodeManager::Dump()
	{
		CameraNodeManager* pMan = CameraNodeManager::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseDump();
	}

	void CameraNodeManager::UpdateCamera()
	{
		CameraNodeManager* pMan = CameraNodeManager::privGetInstance();

		if ((GetKeyState('0') & 0x8000))
		{
			CameraNode* A = pMan->Find(Camera::Name::Default);

			//Trace::out("CurrentCam is %s\n", A->GetName());

			pMan->SetMainCam(A->GetCamera());

			CameraUtility::RememberOrginSet(pMan->GetMainCam());
		}

		if ((GetKeyState('1') & 0x8000))
		{
			CameraNode* A = pMan->Find(Camera::Name::CAMERA_A);

			//Trace::out("CurrentCam is %s\n", A->GetName());

			pMan->SetMainCam(A->GetCamera());

			CameraUtility::RememberOrginSet(pMan->GetMainCam());
		}

		if ((GetKeyState('2') & 0x8000))
		{
			CameraNode* A = pMan->Find(Camera::Name::CAMERA_B);

			//Trace::out("CurrentCam is %s\n", A->GetName());

			pMan->SetMainCam(A->GetCamera());

			CameraUtility::RememberOrginSet(pMan->GetMainCam());
		}

		if ((GetKeyState('3') & 0x8000))
		{
			CameraNode* A = pMan->Find(Camera::Name::CAMERA_C);

			//Trace::out("CurrentCam is %s\n", A->GetName());

			pMan->SetMainCam(A->GetCamera());

			CameraUtility::RememberOrginSet(pMan->GetMainCam());
		}

		if ((GetKeyState('4') & 0x8000))
		{
			CameraNode* A = pMan->Find(Camera::Name::CAMERA_D);

			//Trace::out("CurrentCam is %s\n", A->GetName());

			pMan->SetMainCam(A->GetCamera());

			CameraUtility::RememberOrginSet(pMan->GetMainCam());
		}

		if ((GetKeyState('5') & 0x8000))
		{
			CameraNode* A = pMan->Find(Camera::Name::CAMERA_E);

			//Trace::out("CurrentCam is %s\n", A->GetName());

			pMan->SetMainCam(A->GetCamera());

			CameraUtility::RememberOrginSet(pMan->GetMainCam());
		}


		if ((GetKeyState('6') & 0x8000))
		{
			CameraNode* A = pMan->Find(Camera::Name::CAMERA_F);

			//Trace::out("CurrentCam is %s\n", A->GetName());

			pMan->SetMainCam(A->GetCamera());

			CameraUtility::RememberOrginSet(pMan->GetMainCam());
		}

		if ((GetKeyState('7') & 0x8000))
		{
			CameraNode* A = pMan->Find(Camera::Name::CAMERA_G);

			//Trace::out("CurrentCam is %s\n", A->GetName());

			pMan->SetMainCam(A->GetCamera());

			CameraUtility::RememberOrginSet(pMan->GetMainCam());
		}

		if ((GetKeyState('8') & 0x8000))
		{
			CameraNode* A = pMan->Find(Camera::Name::CAMERA_L);

			//Trace::out("CurrentCam is %s\n", A->GetName());

			pMan->SetMainCam(A->GetCamera());

			CameraUtility::RememberOrginSet(pMan->GetMainCam());
		}


		pMan->poMainCam->Update();
	}

	void CameraNodeManager::UpdateAspectRatio(float ratio)
	{
		CameraNodeManager* instance = CameraNodeManager::privGetInstance();
		
		Iterator* itr = instance->poActive->GetIterator();
		for (itr->First(); !itr->IsDone(); itr->Next())
		{
			CameraNode* cameraNode = (CameraNode*)itr->Curr();
			cameraNode->GetCamera()->SetAspectRatio(ratio);
		}
	}


	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
	CameraNodeManager* CameraNodeManager::privGetInstance()
	{
		// Safety - this forces users to call Create() first before using class
		assert(posInstance != nullptr);
		return posInstance;
	}

	//----------------------------------------------------------------------
	// Override Abstract methods
	//----------------------------------------------------------------------
	DLink* CameraNodeManager::derivedCreateNode()
	{
		DLink* pNodeBase = new CameraNode();
		assert(pNodeBase != nullptr);

		return pNodeBase;
	}
}
