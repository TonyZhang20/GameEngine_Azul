//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Azul
{
	// Do your magic here

	Trans::Trans()
	{
		this->set(Identity);
	}

	Trans::Trans(const float tx, const float ty, const float tz)
	{
		this->set(Identity);

		this->_m12 = tx;
		this->_m13 = ty;
		this->_m14 = tz;
	}

	Trans::Trans(const Vec3& vTrans)
	{
		this->set(Identity);

		this->_m12 = vTrans.x();
		this->_m13 = vTrans.y();
		this->_m14 = vTrans.z();
	}

	void Trans::set(const float tx, const float ty, const float tz)
	{
		this->set(Identity);

		this->_m12 = tx;
		this->_m13 = ty;
		this->_m14 = tz;
	}

	void Trans::set(const Vec3& vTrans)
	{
		this->set(Identity);

		this->_m12 = vTrans.x();
		this->_m13 = vTrans.y();
		this->_m14 = vTrans.z();
	}

	Trans::Trans(const enum Identity_enum)
	{
		this->set(Identity);
	}

	void Trans::set(const enum Identity_enum)
	{
		this->_v0.set(1, 0, 0, 0);
		this->_v1.set(0, 1, 0, 0);
		this->_v2.set(0, 0, 1, 0);
		this->_v3.set(0, 0, 0, 1);
	}


}

// ---  End of File ---
