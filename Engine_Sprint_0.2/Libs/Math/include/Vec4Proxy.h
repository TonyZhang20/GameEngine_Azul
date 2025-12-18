//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_VEC4_PROXY_H
#define ENGINE_MATH_VEC4_PROXY_H

#include "Math_DLLInterface.h"

namespace Azul
{
	// Vector related proxies here:

	struct Vec4Proxy
	{
	public:

		// Suggested inteface:

		MATH_LIBRARY_API Vec4Proxy(float a, float b, float c, float d);

		MATH_LIBRARY_API Vec4Proxy() = delete;
		MATH_LIBRARY_API Vec4Proxy(const Vec4Proxy&) = default;
		MATH_LIBRARY_API Vec4Proxy& operator = (const Vec4Proxy&) = default;
		MATH_LIBRARY_API ~Vec4Proxy() = default;

		MATH_LIBRARY_API operator float() const;

		MATH_LIBRARY_API float operator * (const Vec4Proxy& r) const;

		MATH_LIBRARY_API bool operator > (const Vec4Proxy& r) const;
		MATH_LIBRARY_API bool operator < (const Vec4Proxy& r) const;
		MATH_LIBRARY_API bool operator == (const Vec4Proxy& r) const;

		MATH_LIBRARY_API bool operator >= (const Vec4Proxy& r) const;
		MATH_LIBRARY_API bool operator <= (const Vec4Proxy& r) const;
		MATH_LIBRARY_API bool operator != (const Vec4Proxy& r) const;

	private:
		float x;
		float y;
		float z;
		float w;
	};

}

#endif

//--- End of File ---
