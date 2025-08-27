//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include <math.h>    // <--- ONLY location for <math.h> allowed
#include "Trig.h"

namespace Azul
{

	// Do your magic here

	float Trig::cos(const float angle_radians)
	{
		return std::cos(angle_radians);
	}

	float Trig::sin(const float angle_radians)
	{
		return std::sin(angle_radians);
	}

	float Trig::tan(const float val)
	{
		return std::tan(val);
	}

	float Trig::atan(const float val)
	{
		return std::atan(val);
	}

	float Trig::atan2(const float x, const float y)
	{
		return std::atan2(x, y);
	}

	float Trig::acos(const float val)
	{
		return std::acos(val);
	}

	float Trig::asin(const float val)
	{
		return std::asin(val);
	}

	void Trig::cossin(float& cosOut, float& sinOut, const float angle_radians)
	{
		cosOut = std::cos(angle_radians);
		sinOut = std::sin(angle_radians);
	}

	float Trig::sqrt(const float val)
	{
		return std::sqrt(val);
	}

	float Trig::rsqrt(const float val)
	{
		assert(val > 0);
		return 1 / sqrt(val);
	}

	float Trig::degreesToRadians(float degrees)
	{
		return degrees * (3.14159265358979323846f / 180.0f);
	}

	float Trig::radiansToDegrees(float radians)
	{
		return radians * (180.0f / 3.14159265358979323846f);
	}



}

//--- End of File ---
