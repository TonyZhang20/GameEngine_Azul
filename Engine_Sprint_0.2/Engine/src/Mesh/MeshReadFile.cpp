#include "MeshReadFile.h"
#include "MathEngine.h"
#include "StateDirectXMan.h"
#include "File.h"

namespace Azul
{
	MeshReadFile::MeshReadFile(const char* fileName)
		: Mesh(),
		VBVBuffer_pos(),
		VBVBuffer_norm(),
		VBVBuffer_tex(),
		IBVBuffer()
	{


		char fullPath[256];
		DWORD fileSize = 0;
		char* data = nullptr;

		unsigned int NumVerts = 0;
		unsigned int NumIndices = 0;

#pragma region Load Pos

		sprintf_s(fullPath, sizeof(fullPath), "%s_POSITION_Center.cdm", fileName);
		ReadFile(fullPath, data, fileSize);
	
		NumVerts = fileSize / sizeof( VertexPos );

		VBVBuffer_pos.Initialize(fileSize, sizeof(VertexPos), data);
		delete[] data;
		data = nullptr;

#pragma endregion

#pragma region Load Normal

		sprintf_s(fullPath, sizeof(fullPath), "%s_NORMAL.cdm", fileName);
		ReadFile(fullPath, data, fileSize);

		VBVBuffer_norm.Initialize(fileSize, sizeof(Vec3f), data);

		delete[] data;
		data = nullptr;

#pragma endregion

#pragma region Load Texture

		sprintf_s(fullPath, sizeof(fullPath), "%s_TEXCOORD_0.cdm", fileName);
		ReadFile(fullPath, data, fileSize);

		VBVBuffer_tex.Initialize(fileSize, sizeof(Vec2f), data);
		delete[] data;
		data = nullptr;

#pragma endregion

#pragma region Load Indices

		sprintf_s(fullPath, sizeof(fullPath), "%s_INDICES.cdm", fileName);
		ReadFile(fullPath, data, fileSize);

		NumIndices = fileSize / sizeof(uint32_t);

		IBVBuffer.Initialize(fileSize, data);
		delete[] data;
		data = nullptr;

#pragma endregion

		this->Initialize(NumVerts, NumIndices);

	}



	MeshReadFile::~MeshReadFile()
	{

	}

	void MeshReadFile::ReadFile(const char* const fullPath, char*& data, DWORD& fileSize)
	{
		File::Handle FileHandle;
		File::Error ferror;

		ferror = File::Open(FileHandle, fullPath, File::Mode::READ, false);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Seek(FileHandle, File::Position::END, 0);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Tell(FileHandle, fileSize);
		assert(ferror == File::Error::SUCCESS);

		data = new char[fileSize + 1];

		ferror = File::Seek(FileHandle, File::Position::BEGIN, 0);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(FileHandle, data, fileSize);
		assert(ferror == File::Error::SUCCESS);

		File::Close(FileHandle);
	}

	void MeshReadFile::ActivateMesh()
	{
		this->VBVBuffer_pos.SetActive(VertexSlot::Position);
		this->VBVBuffer_norm.SetActive(VertexSlot::Norm);
		this->VBVBuffer_tex.SetActive(VertexSlot::TexCoord);
	}
	void MeshReadFile::RenderIndexBuffer()
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
}

