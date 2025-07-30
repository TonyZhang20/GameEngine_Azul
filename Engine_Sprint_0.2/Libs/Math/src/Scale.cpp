//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Azul
{
	// Do your magic here

	Scale::Scale()
	{
		this->set(Identity);
	}

	Scale::Scale(const float sx, const float sy, const float sz)
	{
		this->_m0 = sx;
		this->_m5 = sy;
		this->_m10 = sz;
		this->_m15 = 1;
	}

	Scale::Scale(const Vec3& vScale)
	{
		this->_m0 = vScale.x();
		this->_m5 = vScale.y();
		this->_m10 = vScale.z();
		this->_m15 = 1;
	}

	void Scale::set(const float sx, const float sy, const float sz)
	{
		this->_m0 = sx;
		this->_m5 = sy;
		this->_m10 = sz;
	}

	void Scale::set(const Vec3& vScale)
	{
		this->_m0 = vScale.x();
		this->_m5 = vScale.y();
		this->_m10 = vScale.z();
	}

	void Scale::set(const enum Identity_enum)
	{
		this->_v0.set(1, 0, 0, 0);
		this->_v1.set(0, 1, 0, 0);
		this->_v2.set(0, 0, 1, 0);
		this->_v3.set(0, 0, 0, 1);
	}

	Scale::Scale(const enum Identity_enum)
	{
		this->_v0.set(1, 0, 0, 0);
		this->_v1.set(0, 1, 0, 0);
		this->_v2.set(0, 0, 1, 0);
		this->_v3.set(0, 0, 0, 1);
	}
}

// ---  End of File ---
