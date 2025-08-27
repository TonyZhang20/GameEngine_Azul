//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MODEL_H
#define MODEL_H

#include <d3d11.h>
#include "MathEngine.h"
#include "Camera.h"
#include "DLink.h"
#include "BufferCBV_vs.h"

namespace Azul
{
	// Vertex data for a colored cube.
	struct VertexColor
	{
		Vec3f Color;
	};

	struct VertexPos
	{
		Vec3f Position;
	};

	struct VertexNorm
	{
		Vec3f Norm;
	};

	struct VertexTexCoord
	{
		Vec2f TexCoord;
	};

	class Mesh
	{
	public:
		enum class Name
		{
			CUBE = 0,
			PYRAMID,
			A,
			B,
			C,
			D,
			E,
			NULL_MESH,
			NOT_INITIALIZED
		};

	public:
		Mesh();
		Mesh(Mesh& copyModel) = delete;
		Mesh& operator = (Mesh& copyModel) = delete;
		virtual ~Mesh();

		Mesh(unsigned int numVerts, unsigned int numTriangles);

		void Wash();
		bool Compare(DLink* pTarget);
		char* GetName();
		void SetName(Mesh::Name _name);
		void Initialize(unsigned int numVerts, unsigned int numTriangles);

		virtual void ActivateMesh() = 0;
		virtual void RenderIndexBuffer() = 0;

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------
		Name name;

		unsigned int numVerts;
		unsigned int numIndices;
	};

}


#endif

// --- End of File ---
