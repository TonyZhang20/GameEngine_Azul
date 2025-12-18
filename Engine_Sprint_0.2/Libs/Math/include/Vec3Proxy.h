//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_VEC3_PROXY_H
#define ENGINE_MATH_VEC3_PROXY_H

#include "Math_DLLInterface.h"

namespace Azul
{
	// Vector related proxies here:

	struct Vec3Proxy
	{
	public:

		// Suggested inteface:

		MATH_LIBRARY_API Vec3Proxy(float a, float b, float c);
		MATH_LIBRARY_API Vec3Proxy() = delete;
		MATH_LIBRARY_API Vec3Proxy(const Vec3Proxy&) = default;
		MATH_LIBRARY_API Vec3Proxy& operator = (const Vec3Proxy&) = default;
		MATH_LIBRARY_API ~Vec3Proxy() = default;

		MATH_LIBRARY_API operator float() const;

		MATH_LIBRARY_API float operator * (const Vec3Proxy& r) const;

		MATH_LIBRARY_API bool operator > (const Vec3Proxy& r) const;
		MATH_LIBRARY_API bool operator < (const Vec3Proxy& r) const;
		MATH_LIBRARY_API bool operator == (const Vec3Proxy& r) const;
		MATH_LIBRARY_API bool operator >= (const Vec3Proxy& r) const;
		MATH_LIBRARY_API bool operator <= (const Vec3Proxy& r) const;
		MATH_LIBRARY_API bool operator != (const Vec3Proxy& r) const;

	private:
		float x;
		float y;
		float z;
	};

}

#endif

//--- End of File ---
