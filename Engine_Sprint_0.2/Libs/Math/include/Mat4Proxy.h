//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_MAT4_PROXY_H
#define ENGINE_MATH_MAT4_PROXY_H

#include "Math_DLLInterface.h"
#include "Mat4.h"
namespace Azul
{
	// Matrix related proxies here:
	struct Mat4Proxy
	{
		MATH_LIBRARY_API inline Mat4Proxy(Mat4& Mat, float& x)
			: rMat(Mat), rx(x)
		{

		}

		MATH_LIBRARY_API inline Mat4Proxy operator = (float v)
		{
			rx = v;
			rMat._u_m15 &= (unsigned int)(0xFFFFFFF8);

			return *this;
		}

		MATH_LIBRARY_API inline operator float()
		{
			return this->rx;
		}

	private:
		Mat4& rMat;
		float& rx;
	};

}

#endif

//--- End of File ---
