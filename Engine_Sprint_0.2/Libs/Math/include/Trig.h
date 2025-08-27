//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_TRIG_H
#define ENGINE_MATH_TRIG_H
#include "MathExportAPI.h"
namespace Azul
{
	class Trig
	{
	public:

		// Do your magic here

		MATHLIBRARY_API static float cos(const float  angle_radians);
		MATHLIBRARY_API static float sin(const float  angle_radians);
		MATHLIBRARY_API static float tan(const float  val);
		MATHLIBRARY_API static float atan(const float val);
		MATHLIBRARY_API static float atan2(const float x, const float y);
		MATHLIBRARY_API static float acos(const float val );
		MATHLIBRARY_API static float asin(const float val );
		MATHLIBRARY_API static void  cossin(float& cos, float& sin, const float angle_radians);
		MATHLIBRARY_API static float sqrt(const float val);
		MATHLIBRARY_API static float rsqrt(const float val);
		MATHLIBRARY_API static float degreesToRadians(float degrees);
		MATHLIBRARY_API static float radiansToDegrees(float radians);
	};
}

#endif

//--- End of File ---
