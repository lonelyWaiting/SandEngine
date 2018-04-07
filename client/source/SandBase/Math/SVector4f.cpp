#include "SandBase/SandBasePCH.h"
#include "SVector4f.h"
#include "SVector2f.h"
#include "SMatrix4f.h"

SVector4f SVector4f::operator*( const SMatrix4f & mat )
{
	SVector4f result;

	for( int iCol = 0; iCol < 4; iCol++ )
	{
		result[iCol] = 0.0f;
		for( int iRow = 0; iRow < 4; iRow++ )
		{
			result[iCol] += mat( iRow , iCol ) * m[iRow];
		}
	}

	return result;
}

SVector3f SVector4f::xyz() const
{
	return SVector3f( x , y , z );
}

SVector2f SVector4f::xy() const
{
	return SVector2f( x , y );
}