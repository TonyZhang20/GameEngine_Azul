//---------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include "Registration.h"

namespace Azul
{

	Registration::Registration()
	{

	}

	Registration::~Registration()
	{
		set.clear();
	}

	void Registration::RegisterPointer(void* inPointer)
	{
		auto r = set.insert(inPointer);
		//assert(!r.second, "Insert Same Value");
	}

	bool Registration::FindPointer(void* inPointer)
	{
		if (set.empty() == true)
			return false;

		auto r = set.find(inPointer);
		return r != set.end();
	}

	void Registration::UnRegister(void* inPointer)
	{
		set.erase(inPointer);
	}

}

// --- End of File ---

