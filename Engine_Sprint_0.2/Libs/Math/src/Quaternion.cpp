#include "MathEngine.h"

namespace Azul
{
    Quaternion::Quaternion()
    {
        this->_mq = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f); // (x=0,y=0,z=0,w=1)
    }

    Quaternion::Quaternion(const Quaternion& q)
    {
        this->_mq = q._mq;
    }

    Quaternion& Quaternion::operator=(const Quaternion& q)
    {
        if (this != &q)
        {
            this->_mq = q._mq;
        }
        return *this;
    }

    Quaternion::~Quaternion()
    {
    }


    Quaternion::Quaternion(const float x, const float y, const float z, const float w)
    {
        this->_mq = _mm_set_ps(w, z, y, x);
    }

    Quaternion::Quaternion(const Vec3& axis, const float angleRad)
    {
        float half = angleRad * 0.5f;
        float s = Trig::sin(half);
        float c = Trig::cos(half);

        this->_mq = _mm_set_ps(c, axis.z() * s, axis.y() * s, axis.x() * s);
    }

    Quaternion::Quaternion(const Vec4& v)
    {
        this->_mq = v._mv; // assumes Vec4 has same layout
    }

    // Accessors
    void Quaternion::x(const float v) { this->_qx = v; }
    void Quaternion::y(const float v) { this->_qy = v; }
    void Quaternion::z(const float v) { this->_qz = v; }
    void Quaternion::w(const float v) { this->_qw = v; }

    float Quaternion::x() const { return this->_qx; }
    float Quaternion::y() const { return this->_qy; }
    float Quaternion::z() const { return this->_qz; }
    float Quaternion::w() const { return this->_qw; }

    // Addition
    Quaternion Quaternion::operator+(const Quaternion& q) const
    {
        Quaternion result;
        result._mq = _mm_add_ps(this->_mq, q._mq);
        return result;
    }

    Quaternion& Quaternion::operator+=(const Quaternion& q)
    {
        this->_mq = _mm_add_ps(this->_mq, q._mq);
        return *this;
    }

    // Subtraction
    Quaternion Quaternion::operator-(const Quaternion& q) const
    {
        Quaternion result;
        result._mq = _mm_sub_ps(this->_mq, q._mq);
        return result;
    }

    Quaternion& Quaternion::operator-=(const Quaternion& q)
    {
        this->_mq = _mm_sub_ps(this->_mq, q._mq);
        return *this;
    }

    // Scalar multiply
    Quaternion Quaternion::operator*(const float s) const
    {
        Quaternion result;
        __m128 sm = _mm_set1_ps(s);
        result._mq = _mm_mul_ps(this->_mq, sm);
        return result;
    }

    Quaternion& Quaternion::operator*=(const float s)
    {
        __m128 sm = _mm_set1_ps(s);
        this->_mq = _mm_mul_ps(this->_mq, sm);
        return *this;
    }

    Quaternion Quaternion::operator*(const Quaternion& q) const
    {
        Quaternion result;

        result._qx = this->_qw * q._qx + this->_qx * q._qw + this->_qy * q._qz - this->_qz * q._qy;
        result._qy = this->_qw * q._qy - this->_qx * q._qz + this->_qy * q._qw + this->_qz * q._qx;
        result._qz = this->_qw * q._qz + this->_qx * q._qy - this->_qy * q._qx + this->_qz * q._qw;
        result._qw = this->_qw * q._qw - this->_qx * q._qx - this->_qy * q._qy - this->_qz * q._qz;

        return result;
    }

    Quaternion& Quaternion::operator*=(const Quaternion& q)
    {
        *this = *this * q;
        return *this;
    }

    // Functions
    void Quaternion::set(const float x, const float y, const float z, const float w)
    {
        this->_mq = _mm_set_ps(w, z, y, x);
    }

    void Quaternion::set(const Quaternion& q)
    {
        this->_mq = q._mq;
    }

    Quaternion& Quaternion::norm()
    {
        float len = Trig::sqrt(this->dot(*this));
        if (len > MATH_TOLERANCE)
        {
            float inv = 1.0f / len;
            __m128 invV = _mm_set1_ps(inv);
            this->_mq = _mm_mul_ps(this->_mq, invV);
        }
        return *this;
    }

    Quaternion Quaternion::getNorm() const
    {
        Quaternion result(*this);
        result.norm();
        return result;
    }

    Quaternion Quaternion::conjugate() const
    {
        return Quaternion(-this->_qx, -this->_qy, -this->_qz, this->_qw);
    }

    Quaternion Quaternion::inverse() const
    {
        float lenSq = this->dot(*this);
        if (lenSq > MATH_TOLERANCE)
        {
            return this->conjugate() * (1.0f / lenSq);
        }
        return Quaternion();
    }

    float Quaternion::dot(const Quaternion& q) const
    {
        return this->_qx * q._qx + this->_qy * q._qy + this->_qz * q._qz + this->_qw * q._qw;
    }

    Quaternion Quaternion::Identity()
    {
        return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    }

    Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, float t)
    {
        // Compute the cosine of the angle
        float cosTheta = q0.dot(q1);

        Quaternion q1Copy = q1;

        // If cosTheta < 0, negate q1 to take shorter path
        if (cosTheta < 0.0f)
        {
            q1Copy = q1Copy * -1.0f;
            cosTheta = -cosTheta;
        }

        if (cosTheta > 0.9995f)
        {
            // Linear interpolation if very close
            Quaternion result = q0 + (q1Copy - q0) * t;
            return result.getNorm();
        }
        else
        {
            float angle = Trig::acos(cosTheta);
            float s0 = Trig::sin((1.0f - t) * angle);
            float s1 = Trig::sin(t * angle);
            float invSin = 1.0f / Trig::sin(angle);

            Quaternion result = (q0 * s0 + q1Copy * s1) * invSin;
            return result.getNorm();
        }
    }

    void Quaternion::Print(const char* pName) const
    {
        printf("Quaternion %s : (%f , %f , %f , %f)\n", pName, this->_qx, this->_qy, this->_qz, this->_qw);
    }

    MATHLIBRARY_API Vec3 Quaternion::Up() const
    {
        return Vec3(
            2.0f * (_qx * _qy - _qw * _qz),
            1.0f - 2.0f * (_qx * _qx + _qz * _qz),
            2.0f * (_qy * _qz + _qw * _qx)
        );
    }

    MATHLIBRARY_API Vec3 Quaternion::Right() const
    {
        return Vec3(
            1.0f - 2.0f * (_qy * _qy + _qz * _qz),
            2.0f * (_qx * _qy + _qw * _qz),
            2.0f * (_qx * _qz - _qw * _qy)
        );
    }

    MATHLIBRARY_API Vec3 Quaternion::Forward() const
    {
        return Vec3(
            2.0f * (_qx * _qz + _qw * _qy),
            2.0f * (_qy * _qz - _qw * _qx),
            1.0f - 2.0f * (_qx * _qx + _qy * _qy)
        );
    }

    void Quaternion::SetUp(const Vec3& up, const Vec3* forward)
    {
        Vec3 u = up.getNorm();            // 正规化 up
        Vec3 f;

        if (forward)
            f = forward->getNorm();       // 使用传入 forward
        else
            f = this->Forward().getNorm(); // 使用当前 forward

        // 计算正交的 right
        Vec3 r = u.cross(f).getNorm();

        // 重新计算正交 forward
        Vec3 orthoForward = r.cross(u);

        // 构造行优先旋转矩阵
        Mat3 mat = {
            r.x(), r.y(), r.z(),
            u.x(), u.y(), u.z(),
            orthoForward.x(), orthoForward.y(), orthoForward.z()
        };

        // 更新四元数
        SetFromRotationMatrix(mat);
    }

    void Quaternion::SetRight(const Vec3& right, const Vec3* forward)
    {
        Vec3 r = right.getNorm();          // 正规化 right
        Vec3 f;

        if (forward)
            f = forward->getNorm();       // 使用传入 forward
        else
            f = this->Forward().getNorm(); // 使用当前 forward

        // 重新计算正交 up
        Vec3 u = f.cross(r).getNorm();

        // 确保 forward 与 right 正交
        Vec3 orthoForward = r.cross(u);

        // 构造行优先旋转矩阵
        Mat3 mat = {
            r.x(), r.y(), r.z(),
            u.x(), u.y(), u.z(),
            orthoForward.x(), orthoForward.y(), orthoForward.z()
        };

        // 更新四元数
        SetFromRotationMatrix(mat);
    }

    void Quaternion::SetForward(const Vec3& forward, const Vec3* up)
    {
        Vec3 f = forward.getNorm();        // 正规化 forward
        Vec3 u;

        if (up)
            u = up->getNorm();             // 使用传入 up
        else
            u = this->Up().getNorm();      // 使用当前四元数的 up

        // 计算正交的 right 向量
        Vec3 r = u.cross(f).getNorm();

        // 重新计算正交 up 向量
        Vec3 orthoUp = f.cross(r);

        // 构造行优先旋转矩阵
        Mat3 mat = {
            r.x(), r.y(), r.z(),
            orthoUp.x(), orthoUp.y(), orthoUp.z(),
            f.x(), f.y(), f.z()
        };

        // 更新四元数
        SetFromRotationMatrix(mat);
    }

    MATHLIBRARY_API Mat4 Quaternion::ToMatrix() const
    {
        float xx = _qx * _qx; 
        float yy = _qy * _qy; 
        float zz = _qz * _qz; 
        float xy = _qx * _qy; 
        float xz = _qx * _qz; 
        float yz = _qy * _qz; 
        float wx = _qw * _qx; 
        float wy = _qw * _qy; 
        float wz = _qw * _qz;

        Vec4 lineA{ 1.0f - 2.0f * (yy + zz) , 2.0f * (xy - wz) , 2.0f * (xz + wy), 0.0f };
        Vec4 lineB{ 2.0f * (xy + wz) , 1.0f - 2.0f * (xx + zz) , 2.0f * (yz - wx), 0.0f }; 
        Vec4 lineC{ 2.0f * (xz - wy) , 2.0f * (yz + wx) ,1.0f - 2.0f * (xx + yy), 0.0f };
        Vec4 lineD{ 0, 0, 0, 1 };

        return Mat4{ lineA, lineB, lineC, lineD };
    }

    MATHLIBRARY_API void Quaternion::SetFromRotationMatrix(const Mat3 mat)
    {
        float trace = mat[m0] + mat[m5] + mat[m10];
        float qw, qx, qy, qz;

        if (trace > 0.0f)
        {
            float s = Trig::sqrt(trace + 1.0f) * 2.0f;
            qw = 0.25f * s;
            qx = (mat[m9] - mat[m6]) / s;   // m[2][1] - m[1][2]
            qy = (mat[m2] - mat[m8]) / s;   // m[0][2] - m[2][0]
            qz = (mat[m4] - mat[m1]) / s;   // m[1][0] - m[0][1]
        }
        else if ((mat[m0] > mat[m5]) && (mat[m0] > mat[m10]))
        {
            float s = Trig::sqrt(1.0f + mat[m0] - mat[m5] - mat[m10]) * 2.0f;
            qw = (mat[m9] - mat[m6]) / s;
            qx = 0.25f * s;     
            qy = (mat[m1] + mat[m4]) / s;
            qz = (mat[m2] + mat[m8]) / s;
        }
        else if (mat[m5] > mat[m10])
        {
            float s = Trig::sqrt(1.0f + mat[m5] - mat[m0] - mat[m10]) * 2.0f;
            qw = (mat[m2] - mat[m8]) / s;
            qx = (mat[m1] + mat[m4]) / s;
            qy = 0.25f * s;    
            qz = (mat[m6] + mat[m9]) / s;
        }
        else
        {
            float s = Trig::sqrt(1.0f + mat[m10] - mat[m0] - mat[m5]) * 2.0f;
            qw = (mat[m4] - mat[m1]) / s;
            qx = (mat[m2] + mat[m8]) / s;
            qy = (mat[m6] + mat[m9]) / s;
            qz = 0.25f * s;
        }

        _mq = _mm_set_ps(qw, qz, qy, qx);
    }

    MATHLIBRARY_API Vec3 Quaternion::GetUp(const Quaternion& q)
    {
        return q.Up();
    }

    MATHLIBRARY_API Vec3 Quaternion::GetRight(const Quaternion& q)
    {
        return q.Right();
    }

    MATHLIBRARY_API Vec3 Quaternion::GetForward(const Quaternion& q)
    {
        return q.Forward();
    }

}