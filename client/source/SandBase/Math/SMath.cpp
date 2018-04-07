#include "SandBase/SandBasePCH.h"
#include "SVector3f.h"
#include "SMath.h"

using namespace SMath;

#define EPISILON	0.0001f

bool IsZero( SVector3f& v )
{
	if ( v.x > -EPISILON && v.x < EPISILON && v.y > -EPISILON && v.y < EPISILON && v.z > -EPISILON && v.z < EPISILON)
	{
		return true;
	}

	return false;
}