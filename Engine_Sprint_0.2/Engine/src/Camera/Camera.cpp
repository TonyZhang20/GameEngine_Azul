//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "Camera.h"
#include "StringThis.h"

namespace Azul
{
	Camera::Camera()
		: aspectRatio(0), farDist(0), fovy(0), nearDist(0), projMatrix()
	{
	}

	void Camera::GetHelper(Vec3& up, Vec3& tar, Vec3& pos,
		Vec3& upNorm, Vec3& forwardNorm, Vec3& pRightNorm)
	{

	}

	void Camera::SetHelper(Vec3& up_pt, Vec3& tar_pt, Vec3& pos_pt)
	{
		Vec3 upVect = up_pt - pos_pt;
		this->setOrientAndPosition(upVect, tar_pt, pos_pt);
	}


	void Camera::SetAspectRatio(float ratio)
	{
		this->aspectRatio = ratio;
	}

	void Camera::setOrthographic(const float width, const float height, const float nearZ, const float farZ)
	{
		this->aspectRatio = width / height;
		this->nearDist = nearZ;
		this->farDist = farZ;

		// Orthographi（left hand）
		this->projMatrix[m0] = 2.0f / width;
		this->projMatrix[m1] = 0.0f;
		this->projMatrix[m2] = 0.0f;
		this->projMatrix[m3] = 0.0f;

		this->projMatrix[m4] = 0.0f;
		this->projMatrix[m5] = 2.0f / height;
		this->projMatrix[m6] = 0.0f;
		this->projMatrix[m7] = 0.0f;

		this->projMatrix[m8] = 0.0f;
		this->projMatrix[m9] = 0.0f;
		this->projMatrix[m10] = 1.0f / (farZ - nearZ);  // or 2.0 / (farZ - nearZ)
		this->projMatrix[m11] = 0.0f;

		this->projMatrix[m12] = 0.0f;
		this->projMatrix[m13] = 0.0f;
		this->projMatrix[m14] = -nearZ / (farZ - nearZ);  //  -nearZ - farZ 偏移
		this->projMatrix[m15] = 1.0f;

		// DX -> NDC from [-1,1] -> [0,1]
		Trans B(0.0f, 0.0f, 1.0f);
		Scale S(1.0f, 1.0f, 0.5f);
		projMatrix = projMatrix * B * S;
	}

	void Camera::setPerspective(const float Fovy, const float Aspect, const float NearDist, const float FarDist)
	{
		this->aspectRatio = Aspect;
		this->fovy = MATH_PI_180 * Fovy;
		this->nearDist = NearDist;
		this->farDist = FarDist;
	};


	void Camera::setOrientAndPosition(const Vec3& inUp, const Vec3& inLookAt, const Vec3& inPos)
	{

	};

	void Camera::SetFov(const float fov)
	{
		this->fovy = fov;
	}


	void Camera::SetName(Name _name)
	{
		this->name = _name;
	}

	char* Camera::GetName()
	{
		// todo - Hack understand why is this needed for print and fix...
		static char pTmp[128];
		strcpy_s(pTmp, 128, StringMe(this->name));
		return pTmp;
	}

	float& Camera::GetNear()
	{
		return this->nearDist;
	}

	float& Camera::GetFar()
	{
		return this->farDist;
	}

	float& Camera::GetFov()
	{
		return this->fovy;
	}

	// The projection matrix 
	void Camera::privUpdateProjectionMatrix(void)
	{
		float d = 1 / tanf(fovy / 2);

		this->projMatrix[m0] = d / aspectRatio;
		this->projMatrix[m1] = 0.0f;
		this->projMatrix[m2] = 0.0f;
		this->projMatrix[m3] = 0.0f;

		this->projMatrix[m4] = 0.0f;
		this->projMatrix[m5] = d;
		this->projMatrix[m6] = 0.0f;
		this->projMatrix[m7] = 0.0f;

		this->projMatrix[m8] = 0.0f;
		this->projMatrix[m9] = 0.0f;
		this->projMatrix[m10] = -(this->farDist + this->nearDist) / (this->farDist - this->nearDist);
		this->projMatrix[m11] = -1.0f;

		this->projMatrix[m12] = 0.0f;
		this->projMatrix[m13] = 0.0f;
		this->projMatrix[m14] = (-2.0f * this->farDist * this->nearDist) / (this->farDist - this->nearDist);
		this->projMatrix[m15] = 0.0f;

		// Converting from OpenGL-style NDC of [-1,1] to DX's [0,1].  See note: https://anteru.net/blog/2011/12/27/1830/
		// (Note: NDCConvert should be precomputed once and stored for reuse)
		Trans B(0.0f, 0.0f, 1.0f);
		Scale S(1.0f, 1.0f, 0.5f);

		projMatrix = projMatrix * B * S;
	};

	// Update everything (make sure it's consistent)
	void Camera::updateCamera(void)
	{
		// update the projection matrix
		this->privUpdateProjectionMatrix();

		// update the view matrix
	}

	Mat4& Camera::getProjMatrix(void)
	{
		return this->projMatrix;
	}

	Mat4 Camera::getViewMatrix(Quat& q, Vec3& vPos)
	{
		auto right = q.Right();
		auto up = q.Up();
		auto fwd = q.Forward();

		Mat4 viewMatrix{
			Vec4{right[x],  up[x],  fwd[x],  0.0f},
			Vec4{right[y],  up[y],  fwd[y],  0.0f},
			Vec4{right[z],  up[z],  fwd[z],  0.0f},
			Vec4{-vPos.dot(right), -vPos.dot(up), -vPos.dot(fwd), 1.0f}
		};


		return viewMatrix;
	}

}



// --- End of File ---
