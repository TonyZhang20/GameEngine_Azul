//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef Vec3App_H
#define Vec3App_H

#include "Math_DLLInterface.h"

#include "Vec3.h"

namespace Azul
{
	class Vec3App
	{

	public:

		// Suggested Interface

		// lerp
		MATH_LIBRARY_API static void Lerp(Vec3& out, const Vec3& a, const Vec3& b, const float t);

		// lerp array
		MATH_LIBRARY_API static void LerpArray(Vec3* out, const Vec3* a, const Vec3* b, const float t, const int numVects);


	};
}


#endif 

/**** END of VectApp.h *******************************************************/
