#include "SandBase/SandBasePCH.h"
#include "SVector3f.h"
#include <cstdlib>
#include "SMatrix3f.h"
#include "SMath.h"

void SVector3f::Normalize()
{
	float Mag = Magnitude();

	if( 0.0f == Mag )	Mag = 0.0001f;

	float fInvMag = ( 1.0f / Mag );

	x *= fInvMag;
	y *= fInvMag;
	z *= fInvMag;
}

SVector3f SVector3f::operator*( const SMatrix3f & mat )
{
	SVector3f result;

	for( int iCol = 0; iCol < 3; iCol++ )
	{
		result[iCol] = 0.0f;

		for( int iRow = 0; iRow < 3; iRow++ )
		{
			result[iCol] += mat( iRow , iCol ) * m[iRow];
		}
	}
	return result;
}

SVector3f SVector3f::Normalize( const SVector3f& v )
{
	float Mag = v.Magnitude();

	if( 0.0f == Mag )	Mag = 0.0001f;

	float fInvMag = ( 1.0f / Mag );

	return SVector3f( v.x * fInvMag , v.y * fInvMag , v.z * fInvMag );
}

float SVector3f::Magnitude() const
{
	float fLenght = 0.0f;

	fLenght += x * x;
	fLenght += y * y;
	fLenght += z * z;

	return ( sqrtf( fLenght ) );
}

SVector3f SVector3f::Perpendicular()
{
	float xAbs = ( float )fabs( x );
	float yAbs = ( float )fabs( y );
	float zAbs = ( float )fabs( z );

	float minVal = SMath::min<float>(SMath::min<float>(xAbs, yAbs), zAbs);

	// ��xAbsΪ��Сֵ����ҪôxAbsΪ0.0f
	// ҪôxAbs��0.0f�󣬵�������������
	// ���xAbs��0.0f����˵��������y��zƽ��
	// ��(1.0f , 0.0f , 0.0f)���䲻���ߣ�
	// ��Cross�õ��Ľ�����Ǻ�ԭ������ֱ��
	// ���xAbs����0.0f,��ȴ����Сֵ��
	// ��ô˵���������϶�������(1.0f , 0.0f , 0.0f)����
	// ����Cross�õ��Ľ��Ҳ����������Ҫ��
	// yAbs,zAbsΪ��Сֵ�����ͬ��
	if( xAbs == minVal )
	{
		return cross( *this , SVector3f( 1.0f , 0.0f , 0.0f ) );
	}
	else if( yAbs == minVal )
	{
		return cross( *this , SVector3f( 0.0f , 1.0f , 0.0f ) );
	}
	else
	{
		return cross( *this , SVector3f( 0.0f , 0.0f , 1.0f ) );
	}
}

SVector3f SVector3f::Random()
{
	// ��Χ��-1.0f ~ 1.0f
	float x = static_cast< float >( ( double )std::rand() / RAND_MAX ) * 2.0f - 1.0f;
	float y = static_cast< float >( ( double )std::rand() / RAND_MAX ) * 2.0f - 1.0f;
	float z = static_cast< float >( ( double )std::rand() / RAND_MAX ) * 2.0f - 1.0f;

	SVector3f random = SVector3f( x , y , z );
	random.Normalize();

	return ( random );
}

SVector3f SVector3f::Normalize( SVector3f& A )
{
	A.Normalize();

	return A;
}