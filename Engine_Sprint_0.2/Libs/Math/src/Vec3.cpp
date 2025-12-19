//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Azul
{
	// Do your magic here
	Vec3::Vec3()
	{
		this->_mv = _mm_setzero_ps();
	}

	Vec3::Vec3(const Vec3& inV)
	{
		this->_mv = inV._mv;
	}

	Vec3& Vec3::operator=(const Vec3& v)
	{
		if (this != &v)
		{
			this->_mv = v._mv;
		}
		return *this;
	}

	Vec3::~Vec3()
	{
	}


	Vec3::Vec3(const float in_x, const float in_y, const float in_z)
	{
		_mv = _mm_set_ps(0.0f, in_z, in_y, in_x);
	}

	Vec3::Vec3(const Vec4& v)
	{
		this->_vx = v._vx;
		this->_vy = v._vy;
		this->_vz = v._vz;
	}

	Vec3& Vec3::operator=(const Vec4& v)
	{
		// TODO: insert return statement here
		this->_mv = _mm_set_ps(0.0f, v._vz, v._vy, v._vx);
		return *this;
	}

	float& Vec3::operator[](const x_enum)
	{
		return this->_vx;
	}

	float& Vec3::operator[](const y_enum)
	{
		return this->_vy;
	}

	float& Vec3::operator[](const z_enum)
	{
		return this->_vz;
	}

	float Vec3::operator[](const x_enum) const
	{
		return this->_vx;
	}

	float Vec3::operator[](const y_enum) const
	{
		return this->_vy;
	}

	float Vec3::operator[](const z_enum) const
	{
		return this->_vz;
	}

	void Vec3::x(const float v)
	{
		this->_vx = v;
	}

	void Vec3::y(const float v)
	{
		this->_vy = v;
	}

	void Vec3::z(const float v)
	{
		this->_vz = v;
	}

	float Vec3::x() const
	{
		return this->_vx;
	}

	float Vec3::y() const
	{
		return this->_vy;
	}

	float Vec3::z() const
	{
		return this->_vz;
	}

	Vec3 Vec3::operator+(void) const
	{
		return Vec3(*this);
	}

	Vec3 Vec3::operator+(const Vec3& inV) const
	{
		Vec3 result;
		result._mv = _mm_add_ps(this->_mv, inV._mv);

		return result;
	}

	Vec3& Vec3::operator+=(const Vec3& inV)
	{
		// TODO: insert return statement here
		this->_mv = _mm_add_ps(this->_mv, inV._mv);
		return *this;
	}

	Vec3 Vec3::operator-(const Vec3& inV) const
	{
		Vec3 result;
		result._mv = _mm_sub_ps(this->_mv, inV._mv);

		return result;
	}

	Vec3& Vec3::operator-=(const Vec3& inV)
	{
		// TODO: insert return statement here
		this->_mv = _mm_sub_ps(this->_mv, inV._mv);
		return *this;
	}

	Vec3 Vec3::operator-(void) const
	{
		return Vec3(-this->_vx, -this->_vy, -this->_vz);
	}

	Vec3 Vec3::operator*(const float scale) const
	{
		Vec3 result;
		__m128 scaleVec = _mm_set1_ps(scale);
		result._mv = _mm_mul_ps(this->_mv, scaleVec);

		return result;
	}

	Vec3& Vec3::operator*=(const float scale)
	{
		__m128 scaleVec = _mm_set1_ps(scale);
		this->_mv = _mm_mul_ps(this->_mv, scaleVec);
		return *this;
	}

	Vec3 Vec3::operator*(const Mat3& m) const
	{
		return Vec3(
			_vx * m._m0 + _vy * m._m4 + _vz * m._m8,
			_vx * m._m1 + _vy * m._m5 + _vz * m._m9,
			_vx * m._m2 + _vy * m._m6 + _vz * m._m10
		);
	}

	Vec3& Vec3::operator*=(const Mat3& m)
	{
		*this = *this * m;
		return *this;
	}

	Vec3 Vec3::operator* (const Quat& q) const
	{
		Vec3 out;
		q.Lqcvq(*this, out);

		return out;
	}

	Vec3& Vec3::operator*= (const Quat& q)
	{
		Vec3 out;
		q.Lqcvq(*this, out);

		*this = out;
		return *this;
	}

	float Vec3::Distance(const Vec3& vIn)
	{
		float dx = _vx - vIn._vx;
		float dy = _vy - vIn._vy;
		float dz = _vz - vIn._vz;

		return std::sqrt(dx * dx + dy * dy + dz * dz);
	}

	Vec3& Vec3::norm(void)
	{
		float len = this->len();
		if (len == 0)
			this->_mv = _mm_set_ps(0, 0, 0, 0);
		else
			this->_mv = _mm_set_ps(0, _vz / len, _vy / len, _vx / len);

		return *this;
	}

	Vec3 Vec3::getNorm(void) const
	{
		Vec3 result;
		if (this->isZero())
			return result;

		float len = this->len();
		result._mv = _mm_set_ps(0, _vz / len, _vy / len, _vx / len);

		return result;
	}

	float Vec3::dot(const Vec3& vIn) const
	{
		return this->_vx * vIn._vx +
			this->_vy * vIn._vy +
			this->_vz * vIn._vz;
	}

	Vec3 Vec3::cross(const Vec3& vIn) const
	{
		float cx = _vy * vIn._vz - _vz * vIn._vy;
		float cy = _vz * vIn._vx - _vx * vIn._vz;
		float cz = _vx * vIn._vy - _vy * vIn._vx;

		return Vec3(cx, cy, cz);
	}

	Vec3Proxy Vec3::len(void) const
	{
		return Vec3Proxy(this->_vx, this->_vy, this->_vz);
	}

	float Vec3::getAngle(const Vec3& vIn) const
	{
		float dotProduct = this->dot(vIn);
		float lenA = this->len();
		float lenB = vIn.len();

		float lenProduct = lenA * lenB;

		if (lenProduct == 0.0f)
			return 0.0f;

		float cosTheta = dotProduct / lenProduct;

		if (cosTheta > 1.0f) cosTheta = 1.0f;
		if (cosTheta < -1.0f) cosTheta = -1.0f;

		return std::acos(cosTheta);
	}

	void Vec3::set(const float inX, const float inY, const float inZ)
	{
		this->_mv = _mm_set_ps(0, inZ, inY, inX);
	}

	void Vec3::set(const Vec3& A)
	{
		this->_mv = _mm_set_ps(0, A._vz, A._vy, A._vx);
	}

	void Vec3::set(const Vec4& A)
	{
		this->_mv = _mm_set_ps(0, A._vz, A._vy, A._vx);
	}

	bool Vec3::isEqual(const Vec3& v, const float epsilon) const
	{
		return Util::isEqual(this->_vx, v._vx, epsilon) &&
			Util::isEqual(this->_vy, v._vy, epsilon) &&
			Util::isEqual(this->_vz, v._vz, epsilon);
	}

	bool Vec3::isZero(const float epsilon) const
	{
		return Util::isEqual(this->_vx, 0.0f, epsilon) &&
			Util::isEqual(this->_vy, 0.0f, epsilon) &&
			Util::isEqual(this->_vz, 0.0f, epsilon);
	}

	void Vec3::Print(const char* pName) const
	{
		Trace::out("(Vec3 : %s)\n", pName);
		Trace::out("(%f , %f, %f)\n", this->_vx, this->_vy, this->_vz);
	}


	Vec3 operator*(const float scale, const Vec3& inV)
	{
		Vec3 result;
		__m128 scaleVec = _mm_set1_ps(scale);
		result._mv = _mm_mul_ps(inV._mv, scaleVec);

		return result;
	}

}

//--- End of File ---
