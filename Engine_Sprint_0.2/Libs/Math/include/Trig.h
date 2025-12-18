//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_TRIG_H
#define ENGINE_MATH_TRIG_H

#include "Math_DLLInterface.h"

namespace Azul
{
	class MATH_LIBRARY_API Trig
	{
	public:

		// Do your magic here

		static float cos(const float  angle_radians);
		static float sin(const float  angle_radians);
		static float tan(const float  val);
		static float atan(const float val);
		static float atan2(const float x, const float y);
		static float acos(const float val);
		static float asin(const float val);
		static void  cossin(float& cos, float& sin, const float angle_radians);
		static float sqrt(const float val);
		static float rsqrt(const float val);
		static float degreesToRadians(float degrees);
		static float radiansToDegrees(float radians);
	};
}

#endif

//--- End of File ---
