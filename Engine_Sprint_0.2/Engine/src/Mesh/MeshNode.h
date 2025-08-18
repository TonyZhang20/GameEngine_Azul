//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_NODE_H
#define MESH_NODE_H

#include "DLink.h"
#include "Mesh.h"

namespace Azul
{
	class MeshNode : public DLink
	{
	public:
		MeshNode();
		MeshNode(const Mesh&) = delete;
		MeshNode& operator = (const Mesh&) = delete;
		virtual ~MeshNode();

		void Set(Mesh::Name name, Mesh* pMesh);
		Mesh* GetMesh();

		virtual char* GetName() override;
		virtual void Wash() override;
		virtual bool Compare(DLink* pTargetNode) override;
		virtual void Dump() override;

		void ActivateMesh();
		void RenderIndexBuffer();

	private:
		void privClear();

		//--------------------------------------------------
		// Data
		//--------------------------------------------------
		Mesh* poMesh;
	};
}



#endif

// --- End of File ---
