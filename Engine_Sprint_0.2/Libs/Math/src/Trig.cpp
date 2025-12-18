//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include <math.h>    // <--- ONLY location for <math.h> allowed
#include "Constants.h"
#include "Trig.h"

namespace Azul
{
	// Do your magic here
	float Trig::cos(const float angle_radians)
	{
		return std::cosf(angle_radians);
	}

	float Trig::sin(const float angle_radians)
	{
		return std::sinf(angle_radians);
	}

	float Trig::tan(const float val)
	{
		return std::tanf(val);
	}

	float Trig::atan(const float val)
	{
		return std::atanf(val);
	}

	float Trig::atan2(const float x, const float y)
	{
		return std::atan2f(x, y);
	}

	float Trig::acos(const float val)
	{
		return std::acosf(val);
	}

	float Trig::asin(const float val)
	{
		return std::asinf(val);
	}

	void Trig::cossin(float& cosOut, float& sinOut, const float angle_radians)
	{
		cosOut = std::cosf(angle_radians);
		sinOut = std::sinf(angle_radians);
	}

	float Trig::sqrt(const float val)
	{
		return std::sqrtf(val);
	}

	float Trig::rsqrt(const float val)
	{
		assert(val > 0);
		return 1 / sqrtf(val);
	}

	float Trig::degreesToRadians(float degrees)
	{
		return degrees * MATH_PI_180;
	}

	float Trig::radiansToDegrees(float radians)
	{
		return radians * MATH_180_PI;
	}
}

//--- End of File ---
