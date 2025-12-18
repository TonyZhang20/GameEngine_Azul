//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef QuatApp_H
#define QuatApp_H

#include "Math_DLLInterface.h"

#include "Quat.h"

namespace Azul
{
	class QuatApp
	{
	public:
		MATH_LIBRARY_API static void Slerp(Quat &result, const Quat &source, const Quat &target, const float slerpFactor);

		MATH_LIBRARY_API static void SlerpArray(Quat *result, const Quat *source, const Quat *target, const float slerpFactor, const int numQuats);
	};
}

#endif 

// ---  End of File ---------------
