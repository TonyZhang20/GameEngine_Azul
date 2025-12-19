//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "textureData.h"

textureData::~textureData()
{
	delete[] this->poData;
	this->poData = nullptr;
}

textureData::textureData(bool                _enabled,
						 TEXTURE_TYPE        _textType,
						 TEXTURE_MAG_FILTER  _magFilter,
						 TEXTURE_MIN_FILTER  _minFilter,
						 TEXTURE_WRAP        _wrapS,
						 TEXTURE_WRAP        _wrapT,
						 TEXTURE_EFORMAT     _eFormat,
						 TEXTURE_NCOMPONENT  _nComponent,
						 unsigned int       _width,
						 unsigned int       _height,
						 unsigned int       _component,
						 unsigned int       _bits,
						 bool                _as_is,
						 TEXTURE_COMPONENT   _pixel_type,
						 const char *_pName,
						 unsigned int        _dataSize,
						 unsigned char *_poData)
	: pVersion{0},
	enabled{_enabled},
	textType{_textType},
	magFilter{_magFilter},
	minFilter{_minFilter},
	wrapS{_wrapS},
	wrapT{_wrapT},
	eFormat{_eFormat},
	nComponent{_nComponent},
	width{_width},
	height{_height},
	component{_component},
	bits{_bits},
	as_is{_as_is},
	pixel_type{_pixel_type},
	pFileName{0}
{
	memcpy_s(this->pFileName, FILE_NAME_SIZE, _pName, FILE_NAME_SIZE - 1);
	this->dataSize = _dataSize;
	this->poData = new unsigned char[this->dataSize]();
	assert(this->poData);
	memcpy_s(this->poData, this->dataSize, _poData, _dataSize);
}

textureData::textureData()
	: pVersion{0},
	enabled{false},
	textType{TEXTURE_TYPE::DEFAULT},
	magFilter{TEXTURE_MAG_FILTER::DEFAULT},
	minFilter{TEXTURE_MIN_FILTER::DEFAULT},
	wrapS{TEXTURE_WRAP::DEFAULT},
	wrapT{TEXTURE_WRAP::DEFAULT},
	eFormat{TEXTURE_EFORMAT::DEFAULT},
	nComponent{TEXTURE_NCOMPONENT::DEFAULT},
	width{0},
	height{0},
	component{0},
	bits{0},
	as_is{false},
	pixel_type{TEXTURE_COMPONENT::DEFAULT},
	pFileName{0},
	dataSize{0},
	poData{nullptr}
{
	strcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, protoVersion::VERSION_STRING);
	assert(strlen(this->pVersion) < protoVersion::VERSION_NUM_BYTES);
}

textureData::textureData(const textureData &r)
	: pVersion{0},
	enabled{r.enabled},
	textType{r.textType},
	magFilter{r.magFilter},
	minFilter{r.minFilter},
	wrapS{r.wrapS},
	wrapT{r.wrapT},
	eFormat{r.eFormat},
	nComponent{r.nComponent},
	width{r.width},
	height{r.height},
	component{r.component},
	bits{r.bits},
	as_is{r.as_is},
	pixel_type{r.pixel_type},
	pFileName{0},
	dataSize{r.dataSize},
	poData{nullptr}

{
	strcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, protoVersion::VERSION_STRING);
	assert(strlen(this->pVersion) < protoVersion::VERSION_NUM_BYTES);

	memcpy_s(this->pFileName, FILE_NAME_SIZE, r.pFileName, FILE_NAME_SIZE);

	this->poData = new unsigned char[this->dataSize]();
	assert(this->poData);
	memcpy_s(this->poData, this->dataSize, r.poData, r.dataSize);

}

textureData &textureData::operator = (const textureData &r)
{
	if(this != &r)
	{
		delete[] this->poData;
		this->poData = nullptr;

		this->enabled = r.enabled;
		this->textType = r.textType;
		this->magFilter = r.magFilter;
		this->minFilter = r.minFilter;
		this->wrapS = r.wrapS;
		this->wrapT = r.wrapT;
		this->eFormat = r.eFormat;
		this->nComponent = r.nComponent;
		this->width = r.width;
		this->height = r.height;
		this->component = r.component;
		this->bits = r.bits;
		this->dataSize = r.dataSize;
		this->as_is = r.as_is;
		this->pixel_type = r.pixel_type;

		memcpy_s(this->pFileName, FILE_NAME_SIZE, r.pFileName, FILE_NAME_SIZE);

		this->poData = new unsigned char[this->dataSize]();
		assert(this->poData);
		memcpy_s(this->poData, this->dataSize, r.poData, r.dataSize);
	}

	return *this;
}

void textureData::Serialize(textureData_proto &out) const
{
	std::string sVersion((const char *)this->pVersion, protoVersion::VERSION_NUM_BYTES);
	out.set_pversion(sVersion);

	out.set_enabled(this->enabled);
	out.set_texttype((textureData_proto_TEXTURE_TYPE)this->textType);
	out.set_magfilter((textureData_proto_TEXTURE_MAG_FILTER)this->magFilter);
	out.set_minfilter((textureData_proto_TEXTURE_MIN_FILTER)this->minFilter);
	out.set_wraps((textureData_proto_TEXTURE_WRAP)this->wrapS);
	out.set_wrapt((textureData_proto_TEXTURE_WRAP)this->wrapT);
	out.set_eformat((textureData_proto_TEXTURE_EFORMAT)this->eFormat);
	out.set_ncomponent((textureData_proto_TEXTURE_NCOMPONENT)this->nComponent);

	out.set_width(this->width);
	out.set_height(this->height);
	out.set_component(this->component);
	out.set_bits(this->bits);
	out.set_as_is(this->as_is);
	out.set_pixel_type((textureData_proto_TEXTURE_COMPONENT)this->pixel_type);

	// BinaryData 
	// First place "raw" into a string
	//     http://www.cplusplus.com/reference/string/string/string/
	//     from sequence(5): 	string(const char *s, size_t n);
	//     serialize the string

	out.set_filenamesize(FILE_NAME_SIZE);
	std::string s0((const char *)this->pFileName, FILE_NAME_SIZE);
	out.set_filenamedata(s0);

	out.set_datasize(this->dataSize);
	std::string s((const char *)this->poData, this->dataSize);
	out.set_podata(s);
}

void textureData::Deserialize(const textureData_proto &in)
{
	// CHECK the Version
	memcpy_s(this->pVersion, protoVersion::VERSION_NUM_BYTES, in.pversion().data(), protoVersion::VERSION_NUM_BYTES);
	assert(strcmp(this->pVersion, protoVersion::VERSION_STRING) == 0);

	this->enabled = (bool)in.enabled();
	this->textType = (TEXTURE_TYPE)in.texttype();
	this->magFilter = (TEXTURE_MAG_FILTER)in.magfilter();
	this->minFilter = (TEXTURE_MIN_FILTER)in.minfilter();
	this->wrapS = (TEXTURE_WRAP)in.wraps();
	this->wrapT = (TEXTURE_WRAP)in.wrapt();
	this->eFormat = (TEXTURE_EFORMAT)in.eformat();
	this->nComponent = (TEXTURE_NCOMPONENT)in.ncomponent();
	this->width = in.width();
	this->height = in.height();
	this->component = in.component();
	this->bits = in.bits();
	this->as_is = (bool)in.as_is();
	this->pixel_type = (TEXTURE_COMPONENT)in.pixel_type();

	memcpy(this->pFileName, in.filenamedata().data(), in.filenamesize());

	this->dataSize = in.datasize();

	if(this->dataSize > 0)
	{
		this->poData = new unsigned char[this->dataSize]();
		assert(this->poData);
		memcpy(this->poData, in.podata().data(), this->dataSize);
	}

}

void textureData::Print(const char *const pName) const
{
	if(this->enabled)
	{
		Trace::out("%s: \n", pName);
		Trace::out("      dataSize: %d \n", this->dataSize);
		Trace::out("        poData: %2x %2x %2x %2x %2x \n",
				   this->poData[0],
				   this->poData[1],
				   this->poData[2],
				   this->poData[3],
				   this->poData[4]);
		Trace::out("         pName: %s\n", this->pFileName);
		Trace::out("      textType: %d\n", this->textType);
		Trace::out("     magFilter: %d \n", this->magFilter);
		Trace::out("     minFilter: %d\n", this->minFilter);
		Trace::out("         wrapS: %d \n", this->wrapS);
		Trace::out("         wrapT: %d \n", this->wrapT);
		Trace::out("         width: %d \n", this->width);
		Trace::out("        height: %d \n", this->height);
	}

}

// --- End of File ---


