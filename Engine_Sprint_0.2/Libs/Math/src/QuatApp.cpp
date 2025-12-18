//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"
#include "MathApp.h"

//----------------------------------------------------------------------------- 
// Mutates result to be a slerp between the source and target by the
// specified factor.
// For a factor of 0.0, result == source.
// For a factor of 1.0, result == target.
//----------------------------------------------------------------------------- 

namespace Azul
{
	void QuatApp::Slerp(Quat& result, const Quat& source, const Quat& target, const float slerpFactor)
	{
		//tmp = cos(angle) = qsrc · qtar
		const float QUAT_EPSILON = 0.001f;
		float temp = source.dot(target);
		Quat tar = target;

		if (temp >= 1)
		{
			result = source;
		}
		else
		{
			if (temp < 0)
			{
				temp = -temp;
				tar = -target;
			}

			float tarFactor = slerpFactor;
			float srcFactor = 1.0f - slerpFactor;

			if (temp < (1.0f - QUAT_EPSILON)) //aviod cos 1 
			{
				const float omega = Trig::acos(temp);
				const float sinom = 1.0f / Trig::sin(omega);

				srcFactor = Trig::sin(srcFactor * omega) * sinom;
				tarFactor = Trig::sin(tarFactor * omega) * sinom;
			}

			result.set(source[x] * srcFactor + tar[x] * tarFactor,
				source[y] * srcFactor + tar[y] * tarFactor,
				source[z] * srcFactor + tar[z] * tarFactor,
				source[w] * srcFactor + tar[w] * tarFactor);
		}
	}

	void QuatApp::SlerpArray(Quat* result, const Quat* source, const Quat* target, const float slerpFactor, const int numQuats)
	{
		for (int i = 0; i < numQuats; i++)
		{
			Slerp(result[i], source[i], target[i], slerpFactor);
		}
	}
}

// ---  End of File ---------------
