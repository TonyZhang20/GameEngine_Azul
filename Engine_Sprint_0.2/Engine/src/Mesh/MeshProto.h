//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_PROTO_H
#define MESH_PROTO_H

#include "Mesh.h"
#include "BufferIBV.h"
#include "BufferVBV.h"

namespace Azul
{
	class MeshProto : public Mesh
	{
	public:
		// Data
		MeshProto(const char* const pMeshFileName);

		MeshProto() = delete;
		MeshProto(const MeshProto&) = delete;
		MeshProto& operator = (MeshProto&) = delete;
		virtual ~MeshProto();

		virtual void ActivateMesh() override;
		virtual void RenderIndexBuffer() override;

	private:
		// Data
		BufferVBV VBVBuffer_pos;
		BufferVBV VBVBuffer_color;
		BufferVBV VBVBuffer_tex;
		BufferVBV VBVBuffer_norm;

		BufferIBV IBVBuffer;

	};
}

#endif

// --- End of File ---
