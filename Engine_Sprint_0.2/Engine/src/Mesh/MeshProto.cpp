//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MeshProto.h"
#include "MathEngine.h"
#include "StateDirectXMan.h"
#include "ShaderMappings.h"
#include "Engine.h"
#include "File.h"
#include "meshData.h"

namespace Azul
{

	MeshProto::MeshProto(const char* const pMeshFileName)
		: Mesh(0, 0),
		VBVBuffer_pos(),
		VBVBuffer_color(),
		VBVBuffer_tex(),
		VBVBuffer_norm(),
		IBVBuffer()
	{
		// future proofing it for a file
		assert(pMeshFileName);

		unsigned char* poBuff;
		unsigned int numBytes;

		File::Error ferror;
		ferror = File::GetFileAsBuffer(pMeshFileName, poBuff, numBytes);
		assert(ferror == File::Error::SUCCESS);

		// Now the raw data is poBUff
		// deserialize the data --> mB
		std::string strIn((const char*)poBuff, numBytes);
		meshData_proto mB_proto;

		mB_proto.ParseFromString(strIn);

		meshData mB;
		mB.Deserialize(mB_proto);
		delete[] poBuff;

		// Now the model is mB...
		// move it into the Mesh

		// Do I know this format?
		assert(strcmp(mB.pVersion, "2.0.0") == 0);


		// Using the hdr, allocate the space for the buffers
		this->numVerts = (int)mB.vbo_vert.count;
		this->numIndices = (int)mB.triCount * 3;

		assert(this->numIndices > 0);
		assert(this->numVerts > 0);

		// Load the verts data: ---------------------------------------------------------

		// ------------------------------------------------
		// Create an initialize the vertex buffer - pos
		// ------------------------------------------------
		if (mB.vbo_vert.enabled)
		{
			assert(mB.vbo_vert.poData);
			assert(mB.vbo_vert.dataSize > 0);
			assert(mB.vbo_vert.vboType == vboData::VBO_TYPE::VEC3);
			assert(mB.vbo_vert.componentType == vboData::VBO_COMPONENT::FLOAT);

			VBVBuffer_pos.Initialize(mB.vbo_vert.dataSize, mB.vbo_vert.stride, mB.vbo_vert.poData);
		}
		// ------------------------------------------------
		// Create an initialize the vertex buffer - color
		// ------------------------------------------------
		if (mB.vbo_color.enabled)
		{
			assert(mB.vbo_color.poData);
			assert(mB.vbo_color.dataSize > 0);
			assert(mB.vbo_color.vboType == vboData::VBO_TYPE::VEC4);
			assert(mB.vbo_color.componentType == vboData::VBO_COMPONENT::FLOAT);

			VBVBuffer_color.Initialize(mB.vbo_color.dataSize, mB.vbo_color.stride, mB.vbo_color.poData);
		}
		// ------------------------------------------------
		// Create an initialize the vertex buffer - uv
		// ------------------------------------------------
		if (mB.vbo_uv.enabled)
		{
			assert(mB.vbo_uv.poData);
			assert(mB.vbo_uv.dataSize > 0);
			assert(mB.vbo_uv.vboType == vboData::VBO_TYPE::VEC2);
			assert(mB.vbo_uv.componentType == vboData::VBO_COMPONENT::FLOAT);

			VBVBuffer_tex.Initialize(mB.vbo_uv.dataSize, mB.vbo_uv.stride, mB.vbo_uv.poData);
		}
		// ------------------------------------------------
		// Create an initialize the vertex buffer - norms
		// ------------------------------------------------
		if (mB.vbo_norm.enabled)
		{
			assert(mB.vbo_norm.poData);
			assert(mB.vbo_norm.dataSize > 0);
			assert(mB.vbo_norm.vboType == vboData::VBO_TYPE::VEC3);
			assert(mB.vbo_norm.componentType == vboData::VBO_COMPONENT::FLOAT);

			VBVBuffer_norm.Initialize(mB.vbo_norm.dataSize, mB.vbo_norm.stride, mB.vbo_norm.poData);
		}
		// ------------------------------------------------
		// Create and initialize the index buffer.
		// ------------------------------------------------
		if (mB.vbo_index.enabled)
		{
			assert(mB.vbo_index.vboType == vboData::VBO_TYPE::SCALAR);
			assert(mB.vbo_index.componentType != vboData::VBO_COMPONENT::DOUBLE
				&& mB.vbo_index.componentType != vboData::VBO_COMPONENT::FLOAT);

			assert(mB.vbo_index.dataSize > 0);
			assert(mB.vbo_index.poData);

			IBVBuffer.Initialize(mB.vbo_index.dataSize, mB.vbo_index.poData);
		}
	}

	void MeshProto::ActivateMesh()
	{
		// ---------------------------------------------
		//    Set (point to ) vertex: position 
		//    Set (point to ) vertex: color
		//    Set (point to ) vertex: texCoord
		//    Set (point ot ) vertex: norm
		// ---------------------------------------------
		this->VBVBuffer_pos.SetActive(VertexSlot::Position);
		//this->VBVBuffer_color.SetActive(VertexSlot::Color);
		this->VBVBuffer_tex.SetActive(VertexSlot::TexCoord);
		this->VBVBuffer_norm.SetActive(VertexSlot::Norm);
	}

	void MeshProto::RenderIndexBuffer()
	{
		// ---------------------------------------------
		//    Set (point to ) Index buffer 
		//    Render configuration: Triangles
		// ---------------------------------------------
		this->IBVBuffer.SetActive();
		StateDirectXMan::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//--------------------------------------------------------
		// RENDER - using index data
		//--------------------------------------------------------
		StateDirectXMan::GetContext()->DrawIndexed(numIndices, 0, 0);
	}

	MeshProto::~MeshProto()
	{
		// remove anything dynamic here
	}
}

// --- End of File ---
