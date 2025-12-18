//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Azul
{
	// Do your magic here
	Vec4::Vec4()
	{
		this->_mv = _mm_setzero_ps();
	}

	Vec4::Vec4(const Vec4& inV)
	{
		this->_mv = _mm_set_ps(inV._vw, inV._vz, inV._vy, inV._vx);
	}

	Vec4& Vec4::operator=(const Vec4& v)
	{
		if (this != &v)
		{
			this->_mv = v._mv;
		}
		return *this;
	}

	Vec4::~Vec4()
	{
	}

	Vec4::Vec4(const float in_x, const float in_y, const float in_z, const float in_w)
	{
		_mv = _mm_set_ps(in_w, in_z, in_y, in_x);
	}

	Vec4::Vec4(const Vec3& v, const float w)
	{
		_mv = _mm_set_ps(w, v._vz, v._vy, v._vx);
	}

	float& Vec4::operator[](const x_enum)
	{
		return this->_vx;
	}

	float& Vec4::operator[](const y_enum)
	{
		return this->_vy;
	}

	float& Vec4::operator[](const z_enum)
	{
		return this->_vz;
	}

	float& Vec4::operator[] (const w_enum)
	{
		return this->_vw;
	}

	float Vec4::operator[](const x_enum) const
	{
		return this->_vx;
	}

	float Vec4::operator[](const y_enum) const
	{
		return this->_vy;
	}

	float Vec4::operator[](const z_enum) const
	{
		return this->_vz;
	}

	float Vec4::operator[](const w_enum) const
	{
		return this->_vw;
	}

	void Vec4::x(const float v)
	{
		this->_vx = v;
	}

	void Vec4::y(const float v)
	{
		this->_vy = v;
	}

	void Vec4::z(const float v)
	{
		this->_vz = v;
	}

	void Vec4::w(const float v)
	{
		this->_vw = v;
	}

	float Vec4::x() const
	{
		return this->_vx;
	}

	float Vec4::y() const
	{
		return this->_vy;
	}

	float Vec4::z() const
	{
		return this->_vz;
	}

	float Vec4::w() const
	{
		return this->_vw;
	}

	Vec4 Vec4::operator+(void) const
	{
		return Vec4(*this);
	}

	Vec4 Vec4::operator+(const Vec4& inV) const
	{
		Vec4 result;
		result._mv = _mm_add_ps(this->_mv, inV._mv);

		return result;
	}

	Vec4& Vec4::operator+=(const Vec4& inV)
	{
		this->_mv = _mm_add_ps(this->_mv, inV._mv);
		return *this;
	}

	Vec4 Vec4::operator-(const Vec4& inV) const
	{
		Vec4 result;
		result._mv = _mm_sub_ps(this->_mv, inV._mv);

		return result;
	}

	Vec4& Vec4::operator-=(const Vec4& inV)
	{
		this->_mv = _mm_sub_ps(this->_mv, inV._mv);
		return *this;
	}

	Vec4 Vec4::operator-(void) const
	{
		return Vec4(-this->_vx, -this->_vy, -this->_vz, -this->_vw);
	}

	Vec4 Vec4::operator*(const float scale) const
	{
		Vec4 result;
		__m128 scaleVec = _mm_set1_ps(scale);
		result._mv = _mm_mul_ps(this->_mv, scaleVec);

		return result;
	}

	Vec4& Vec4::operator*=(const float scale)
	{
		__m128 scaleVec = _mm_set1_ps(scale);
		this->_mv = _mm_mul_ps(this->_mv, scaleVec);
		return *this;
	}

	Vec3 Vec4::getVec3() const
	{
		return Vec3(this->_vx, this->_vy, this->_vz);
	}

	Vec4 Vec4::operator*(const Mat4& m) const
	{
		return Vec4(
			_vx * m._m0 + _vy * m._m4 + _vz * m._m8 + _vw * m._m12,
			_vx * m._m1 + _vy * m._m5 + _vz * m._m9 + _vw * m._m13,
			_vx * m._m2 + _vy * m._m6 + _vz * m._m10 + _vw * m._m14,
			_vx * m._m3 + _vy * m._m7 + _vz * m._m11 + _vw * m._m15
		);
	}

	Vec4& Vec4::operator*=(const Mat4& m)
	{
		*this = *this * m;
		return *this;
	}

	Vec4& Vec4::norm(void)
	{
		float len = this->len();
		if (len == 0)
			this->_mv = _mm_set_ps(0.f, 0.f, 0.f, 0.f);
		else
			this->_mv = _mm_set_ps(_vw / len, _vz / len, _vy / len, _vx / len);

		return *this;
	}

	Vec4 Vec4::getNorm(void) const
	{
		Vec4 result;
		if (this->isZero())
			return result;

		float len = this->len();
		result._mv = _mm_set_ps(_vw / len, _vz / len, _vy / len, _vx / len);

		return result;
	}

	float Vec4::dot(const Vec4& vIn) const
	{
		return this->_vx * vIn._vx +
			this->_vy * vIn._vy +
			this->_vz * vIn._vz +
			this->_vw * vIn._vw;
	}

	Vec4Proxy Vec4::len() const
	{
		return Vec4Proxy(this->_vx, this->_vy, this->_vz, this->_vw);
	}

	void Vec4::set(const float inX, const float inY, const float inZ, const float inW)
	{
		this->_mv = _mm_set_ps(inW, inZ, inY, inX);
	}

	void Vec4::set(const Vec4& A)
	{
		this->_mv = A._mv;
	}

	void Vec4::set(const Vec3& v, const float w)
	{
		this->_mv = _mm_set_ps(w, v._vz, v._vy, v._vx);
	}

	bool Vec4::isEqual(const Vec4& v, const float epsilon) const
	{
		return Util::isEqual(this->_vx, v._vx, epsilon) &&
			Util::isEqual(this->_vy, v._vy, epsilon) &&
			Util::isEqual(this->_vz, v._vz, epsilon) &&
			Util::isEqual(this->_vw, v._vw, epsilon);
	}

	bool Vec4::isZero(const float epsilon) const
	{
		return Util::isEqual(this->_vx, 0.0f, epsilon) &&
			Util::isEqual(this->_vy, 0.0f, epsilon) &&
			Util::isEqual(this->_vz, 0.0f, epsilon) &&
			Util::isEqual(this->_vw, 0.0f, epsilon);
	}

	void Vec4::Print(const char* pName) const
	{
		printf("Vec4 : %s  \n", pName);
		printf("(%f , %f, %f, %f\n", this->_vx, this->_vy, this->_vz, this->_vw);
	}

	Vec4 operator*(const float scale, const Vec4& inV)
	{
		Vec4 result;
		__m128 scaleVec = _mm_set1_ps(scale);
		result._mv = _mm_mul_ps(inV._mv, scaleVec);

		return result;
	}

}

//--- End of File ---
