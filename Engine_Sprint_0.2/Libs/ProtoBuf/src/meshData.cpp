//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "meshData.h"

meshData::~meshData()
{

}

meshData::meshData()
	:
	pVersion{0},
	meshCount{0},
	meshNames(),
	renderModes(),
	triCounts(),
	vertCounts(),
	vbo_vert(),
	vbo_norm(),
	vbo_uv(),
	vbo_index(),
	vbo_color()
{
	strcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, protoVersion::VERSION_STRING);
	assert(strlen(this->pVersion) < protoVersion::VERSION_NUM_BYTES);
}

void meshData::SetMeshCount(size_t count)
{
	this->meshCount = count;

	meshNames.resize(meshCount);
	renderModes.resize(meshCount);
	triCounts.resize(meshCount);
	vertCounts.resize(meshCount);
	vbo_vert.resize(meshCount);
	vbo_norm.resize(meshCount);
	vbo_uv.resize(meshCount);
	vbo_index.resize(meshCount);
	vbo_color.resize(meshCount);
}

void meshData::Serialize(meshData_proto &out) const
{
	std::string sVersion((const char *)this->pVersion, protoVersion::VERSION_NUM_BYTES);
	out.set_pversion(sVersion);

	out.set_meshcount((uint32_t)this->meshCount);

	for (size_t i = 0; i < meshCount; i++)
	{
		std::string sName((const char*)this->meshNames[i].data(), meshData::FILE_NAME_SIZE);
		out.add_pmeshname(sName);

		out.add_mode((meshData_proto_RENDER_MODE)this->renderModes[i]);
		out.add_tricount(this->triCounts[i]);
		out.add_vertcount(this->vertCounts[i]);


		this->vbo_vert[i].Serialize(*out.add_vbo_vert());
		this->vbo_norm[i].Serialize(*out.add_vbo_norm());
		this->vbo_uv[i].Serialize(*out.add_vbo_uv());
		this->vbo_index[i].Serialize(*out.add_vbo_index());
		this->vbo_color[i].Serialize(*out.add_vbo_color());
		
		//vboData_proto* pVBO_proto;

		/* old version
		//vert
		pVBO_proto = out.add_vbo_vert();
		this->vbo_vert[i].Serialize(*pVBO_proto);

		//norm
		pVBO_proto = out.add_vbo_norm();
		this->vbo_norm[i].Serialize(*pVBO_proto);

		//vbo_uv
		pVBO_proto = out.add_vbo_uv();
		this->vbo_uv[i].Serialize(*pVBO_proto);

		//index
		pVBO_proto = out.add_vbo_index();
		this->vbo_index[i].Serialize(*pVBO_proto);

		//vbo_color
		pVBO_proto = out.add_vbo_color();
		this->vbo_color[i].Serialize(*pVBO_proto);
		*/
	}
}

void meshData::Deserialize(const meshData_proto &in)
{
	// CHECK the Version
	memcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, in.pversion().data(), protoVersion::VERSION_NUM_BYTES);
	assert(strcmp(this->pVersion, protoVersion::VERSION_STRING) == 0);
	
	this->meshCount = in.meshcount();

	this->meshNames.clear();
	this->renderModes.clear();
	this->triCounts.clear();
	this->vertCounts.clear();
	this->vbo_vert.clear();
	this->vbo_norm.clear();
	this->vbo_uv.clear();
	this->vbo_index.clear();
	this->vbo_color.clear();

	for (int i = 0; i < in.meshcount(); i++)
	{
		std::array<char, FILE_NAME_SIZE> meshName;
		memcpy_s(meshName.data(), FILE_NAME_SIZE, in.pmeshname(i).data(), FILE_NAME_SIZE);

		this->meshNames.push_back(meshName);

		this->renderModes.push_back(static_cast<RENDER_MODE>(in.mode(i)));
		this->triCounts.push_back(in.tricount(i));
		this->vertCounts.push_back(in.vertcount(i));

		vboData vertVBO, normVBO, uvVBO, indexVBO, colorVBO;

		if (i < in.vbo_vert_size()) {
			vertVBO.Deserialize(in.vbo_vert(i));
		}
		if (i < in.vbo_norm_size()) {
			normVBO.Deserialize(in.vbo_norm(i));
		}
		if (i < in.vbo_uv_size()) {
			uvVBO.Deserialize(in.vbo_uv(i));
		}
		if (i < in.vbo_index_size()) {
			indexVBO.Deserialize(in.vbo_index(i));
		}
		if (i < in.vbo_color_size()) {
			colorVBO.Deserialize(in.vbo_color(i));
		}


		this->vbo_vert.push_back(vertVBO);
		this->vbo_norm.push_back(normVBO);
		this->vbo_uv.push_back(uvVBO);
		this->vbo_index.push_back(indexVBO);
		this->vbo_color.push_back(colorVBO);
	}
}

void meshData::Print(const char *const pName) const
{
	Trace::out("%s: \n", pName);
	Trace::out("\n");

	Trace::out("version: %s \n", this->pVersion);

	for (size_t i = 0; i < this->meshCount; i++)
	{
		Trace::out("\n");
		this->vbo_vert[i].Print("vbo_vert");
		Trace::out("\n");
		this->vbo_norm[i].Print("vbo_norm");
		Trace::out("\n");
		this->vbo_uv[i].Print("vbo_uv");
		Trace::out("\n");
		this->vbo_index[i].Print("vbo_index");
		Trace::out("\n");

		if (this->vbo_color[i].enabled)
		{
			this->vbo_color[i].Print("vbo_color");
			Trace::out("\n");
		}
	}
}

// --- End of File ---


