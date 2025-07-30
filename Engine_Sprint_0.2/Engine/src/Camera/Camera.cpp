//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "Camera.h"
#include "StringThis.h"

namespace Azul
{
	Camera::Camera()
		: aspectRatio(0), farDist(0), fovy(0), nearDist(0)
	{
	}

	void Camera::GetHelper(Vec3& up, Vec3& tar, Vec3& pos,
		Vec3& upNorm, Vec3& forwardNorm, Vec3& pRightNorm)
	{
		this->getPos(pos);
		this->getLookAt(tar);
		this->getUp(upNorm);
		up = pos + upNorm;

		forwardNorm = tar - pos;
		forwardNorm.norm();

		this->getRight(pRightNorm);
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

	void Camera::setPerspective(const float Fovy, const float Aspect, const float NearDist, const float FarDist)
	{
		this->aspectRatio = Aspect;
		this->fovy = MATH_PI_180 * Fovy;
		this->nearDist = NearDist;
		this->farDist = FarDist;
	};


	void Camera::setOrientAndPosition(const Vec3& inUp, const Vec3& inLookAt, const Vec3& inPos)
	{
		// Remember the up, dir and right are unit length, and are perpendicular.
		// Treat lookAt as king, find Right vect, then correct Up to insure perpendiculare.
		// Make sure that all vectors are unit vectors.

		this->vLookAt = inLookAt;
		this->vDir = -(inLookAt - inPos); // Right-Hand camera: vDir is flipped
		this->vDir.norm();

		// Clean up the vectors (Right hand rule)
		this->vRight = inUp.cross(this->vDir);
		this->vRight.norm();

		this->vUp = this->vDir.cross(this->vRight);
		this->vUp.norm();

		this->vPos = inPos;
	};


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


	void Camera::privUpdateViewMatrix(void)
	{
		// This functions assumes the your vUp, vRight, vDir are still unit
		// And perpendicular to each other

		// Set for DX Right-handed space
		this->viewMatrix[m0] = this->vRight[x];
		this->viewMatrix[m1] = this->vUp[x];
		this->viewMatrix[m2] = this->vDir[x];
		this->viewMatrix[m3] = 0.0f;

		this->viewMatrix[m4] = this->vRight[y];
		this->viewMatrix[m5] = this->vUp[y];
		this->viewMatrix[m6] = this->vDir[y];
		this->viewMatrix[m7] = 0.0f;

		this->viewMatrix[m8] = this->vRight[z];
		this->viewMatrix[m9] = this->vUp[z];
		this->viewMatrix[m10] = this->vDir[z];
		this->viewMatrix[m11] = 0.0f;

		// Apply R^t( -Pos ). Use dot-product with the basis vectors
		this->viewMatrix[m12] = -vPos.dot(vRight);
		this->viewMatrix[m13] = -vPos.dot(vUp);
		this->viewMatrix[m14] = -vPos.dot(vDir);
		this->viewMatrix[m15] = 1.0f;
	};


	// Update everything (make sure it's consistent)
	void Camera::updateCamera(void)
	{
		// update the projection matrix
		this->privUpdateProjectionMatrix();

		// update the view matrix
		this->privUpdateViewMatrix();
	}

	Mat4& Camera::getViewMatrix(void)
	{
		return this->viewMatrix;
	}

	Mat4& Camera::getProjMatrix(void)
	{
		return this->projMatrix;
	}

	void Camera::getPos(Vec3& outPos) const
	{
		outPos = this->vPos;
	}

	void  Camera::getDir(Vec3& outDir) const
	{
		outDir = this->vDir;
	}

	void  Camera::getUp(Vec3& outUp) const
	{
		outUp = this->vUp;
	}

	void Camera::getLookAt(Vec3& outLookAt) const
	{
		outLookAt = this->vLookAt;
	}

	void Camera::getRight(Vec3& outRight) const
	{
		outRight = this->vRight;
	}
}



// --- End of File ---
