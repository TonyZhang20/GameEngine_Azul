//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Azul
{
	// Add Proxy here
	Vec4Proxy::Vec4Proxy(float a, float b, float c, float d)
	{
		this->x = a;
		this->y = b;
		this->z = c;
		this->w = d;
	}

	Vec4Proxy::operator float() const
	{
		return Trig::sqrt(x * x + y * y + z * z + w * w);
	}

	float Vec4Proxy::operator*(const Vec4Proxy& r) const
	{
		float a = *this;
		float b = r;
		return a * b;
	}

	bool Vec4Proxy::operator>(const Vec4Proxy& r) const
	{
		float a = *this;
		float b = r;
		return a > b;
	}

	bool Vec4Proxy::operator<(const Vec4Proxy& r) const
	{
		float a = *this;
		float b = r;
		return a < b;
	}

	bool Vec4Proxy::operator==(const Vec4Proxy& r) const
	{
		float a = *this;
		float b = r;
		return a == b;
	}

	bool Vec4Proxy::operator>=(const Vec4Proxy& r) const
	{
		float a = *this;
		float b = r;
		return a >= b;
	}

	bool Vec4Proxy::operator<=(const Vec4Proxy& r) const
	{
		float a = *this;
		float b = r;
		return a <= b;
	}

	bool Vec4Proxy::operator!=(const Vec4Proxy& r) const
	{
		float a = *this;
		float b = r;
		return a != b;
	}
}

//--- End of File ---
