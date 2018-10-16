#include "SandBase/SandBasePCH.h"
#include "SMatrix4f.h"
#include "SMatrix3f.h"
#include "SVector4f.h"
#include <iostream>

SMatrix4f::SMatrix4f()
{

}

SMatrix4f::SMatrix4f( bool bZero )
{
	if( bZero )
	{
		memset(m_afEntry, 0, 4 * 4 * sizeof(float));
	}
}

SMatrix4f::SMatrix4f( const SMatrix4f& Matrix )
{
	memcpy( m_afEntry , ( void* )&Matrix , 16 * sizeof( float ) );
}

SMatrix4f::SMatrix4f( float fM11 , float fM12 , float fM13 , float fM14 ,
					float fM21 , float fM22 , float fM23 , float fM24 ,
					float fM31 , float fM32 , float fM33 , float fM34 ,
					float fM41 , float fM42 , float fM43 , float fM44 )
{
	m_afEntry[0] = fM11;	m_afEntry[1] = fM12;	m_afEntry[2] = fM13;	m_afEntry[3] = fM14;
	m_afEntry[4] = fM21;	m_afEntry[5] = fM22;	m_afEntry[6] = fM23;	m_afEntry[7] = fM24;
	m_afEntry[8] = fM31;	m_afEntry[9] = fM32;	m_afEntry[10] = fM33;	m_afEntry[11] = fM34;
	m_afEntry[12] = fM41;	m_afEntry[13] = fM42;	m_afEntry[14] = fM43;	m_afEntry[15] = fM44;
}

SMatrix4f& SMatrix4f::operator= ( const SMatrix4f& Matrix )
{
	memcpy( m_afEntry , Matrix.m_afEntry , 16 * sizeof( float ) );

	return ( *this );
}

void SMatrix4f::RotationX( float fRadians )
{
	float fSin = sinf( fRadians );
	float fCos = cosf( fRadians );

	m_afEntry[0] = 1.0f;	m_afEntry[1] = 0.0f;	m_afEntry[2] = 0.0f;	m_afEntry[3] = 0.0f;
	m_afEntry[4] = 0.0f;	m_afEntry[5] = fCos;	m_afEntry[6] = fSin;	m_afEntry[7] = 0.0f;
	m_afEntry[8] = 0.0f;	m_afEntry[9] = -fSin;	m_afEntry[10] = fCos;	m_afEntry[11] = 0.0f;
	m_afEntry[12] = 0.0f;	m_afEntry[13] = 0.0f;	m_afEntry[14] = 0.0f;	m_afEntry[15] = 1.0f;
}

void SMatrix4f::RotationY( float fRadians )
{
	float fSin = sinf( fRadians );
	float fCos = cosf( fRadians );

	m_afEntry[0] = fCos;	m_afEntry[1] = 0.0f;	m_afEntry[2] = -fSin;	m_afEntry[3] = 0.0f;
	m_afEntry[4] = 0.0f;	m_afEntry[5] = 1.0f;	m_afEntry[6] = 0.0f;	m_afEntry[7] = 0.0f;
	m_afEntry[8] = fSin;	m_afEntry[9] = 0.0f;	m_afEntry[10] = fCos;	m_afEntry[11] = 0.0f;
	m_afEntry[12] = 0.0f;	m_afEntry[13] = 0.0f;	m_afEntry[14] = 0.0f;	m_afEntry[15] = 1.0f;
}

void SMatrix4f::RotationZ( float fRadians )
{
	float fSin = sinf( fRadians );
	float fCos = cosf( fRadians );

	m_afEntry[0] = fCos;	m_afEntry[1] = fSin;	m_afEntry[2] = 0.0f;	m_afEntry[3] = 0.0f;
	m_afEntry[4] = -fSin;	m_afEntry[5] = fCos;	m_afEntry[6] = 0.0f;	m_afEntry[7] = 0.0f;
	m_afEntry[8] = 0.0f;	m_afEntry[9] = 0.0f;	m_afEntry[10] = 1.0f;	m_afEntry[11] = 0.0f;
	m_afEntry[12] = 0.0f;	m_afEntry[13] = 0.0f;	m_afEntry[14] = 0.0f;	m_afEntry[15] = 1.0f;
}

void SMatrix4f::Scale( float fScale )
{
	m_afEntry[0] = fScale;	m_afEntry[1] = 0.0f;	m_afEntry[2] = 0.0f;	m_afEntry[3] = 0.0f;
	m_afEntry[4] = 0.0f;	m_afEntry[5] = fScale;	m_afEntry[6] = 0.0f;	m_afEntry[7] = 0.0f;
	m_afEntry[8] = 0.0f;	m_afEntry[9] = 0.0f;	m_afEntry[10] = fScale;	m_afEntry[11] = 0.0f;
	m_afEntry[12] = 0.0f;	m_afEntry[13] = 0.0f;	m_afEntry[14] = 0.0f;	m_afEntry[15] = 1.0f;
}

void SMatrix4f::Translate( float fX , float fY , float fZ )
{
	m_afEntry[0] = 1.0f;	m_afEntry[1] = 0.0f;	m_afEntry[2] = 0.0f;	m_afEntry[3] = 0.0f;
	m_afEntry[4] = 0.0f;	m_afEntry[5] = 1.0f;	m_afEntry[6] = 0.0f;	m_afEntry[7] = 0.0f;
	m_afEntry[8] = 0.0f;	m_afEntry[9] = 0.0f;	m_afEntry[10] = 1.0f;	m_afEntry[11] = 0.0f;
	m_afEntry[12] = fX;		m_afEntry[13] = fY;		m_afEntry[14] = fZ;		m_afEntry[15] = 1.0f;
}

SMatrix4f SMatrix4f::Inverse() const
{
	float fA0 = m_afEntry[0] * m_afEntry[5] - m_afEntry[1] * m_afEntry[4];
	float fA1 = m_afEntry[0] * m_afEntry[6] - m_afEntry[2] * m_afEntry[4];
	float fA2 = m_afEntry[0] * m_afEntry[7] - m_afEntry[3] * m_afEntry[4];
	float fA3 = m_afEntry[1] * m_afEntry[6] - m_afEntry[2] * m_afEntry[5];
	float fA4 = m_afEntry[1] * m_afEntry[7] - m_afEntry[3] * m_afEntry[5];
	float fA5 = m_afEntry[2] * m_afEntry[7] - m_afEntry[3] * m_afEntry[6];
	float fB0 = m_afEntry[8] * m_afEntry[13] - m_afEntry[9] * m_afEntry[12];
	float fB1 = m_afEntry[8] * m_afEntry[14] - m_afEntry[10] * m_afEntry[12];
	float fB2 = m_afEntry[8] * m_afEntry[15] - m_afEntry[11] * m_afEntry[12];
	float fB3 = m_afEntry[9] * m_afEntry[14] - m_afEntry[10] * m_afEntry[13];
	float fB4 = m_afEntry[9] * m_afEntry[15] - m_afEntry[11] * m_afEntry[13];
	float fB5 = m_afEntry[10] * m_afEntry[15] - m_afEntry[14] * m_afEntry[11];

	SMatrix4f kInv;
	kInv( 0 , 0 ) = +m_afEntry[5] * fB5 - m_afEntry[6] * fB4 + m_afEntry[7] * fB3;
	kInv( 1 , 0 ) = -m_afEntry[4] * fB5 + m_afEntry[6] * fB2 - m_afEntry[7] * fB1;
	kInv( 2 , 0 ) = +m_afEntry[4] * fB4 - m_afEntry[5] * fB2 + m_afEntry[7] * fB0;
	kInv( 3 , 0 ) = -m_afEntry[4] * fB3 + m_afEntry[5] * fB1 - m_afEntry[6] * fB0;

	kInv( 0 , 1 ) = -m_afEntry[1] * fB5 + m_afEntry[2] * fB4 - m_afEntry[3] * fB3;
	kInv( 1 , 1 ) = +m_afEntry[0] * fB5 - m_afEntry[2] * fB2 + m_afEntry[3] * fB1;
	kInv( 2 , 1 ) = -m_afEntry[0] * fB4 + m_afEntry[1] * fB2 - m_afEntry[3] * fB0;
	kInv( 3 , 1 ) = +m_afEntry[0] * fB3 - m_afEntry[1] * fB1 + m_afEntry[2] * fB0;

	kInv( 0 , 2 ) = +m_afEntry[13] * fA5 - m_afEntry[14] * fA4 + m_afEntry[15] * fA3;
	kInv( 1 , 2 ) = -m_afEntry[12] * fA5 + m_afEntry[14] * fA2 - m_afEntry[15] * fA1;
	kInv( 2 , 2 ) = +m_afEntry[12] * fA4 - m_afEntry[13] * fA2 + m_afEntry[15] * fA0;
	kInv( 3 , 2 ) = -m_afEntry[12] * fA3 + m_afEntry[13] * fA1 - m_afEntry[14] * fA0;

	kInv( 0 , 3 ) = -m_afEntry[9] * fA5 + m_afEntry[10] * fA4 - m_afEntry[11] * fA3;
	kInv( 1 , 3 ) = +m_afEntry[8] * fA5 - m_afEntry[10] * fA2 + m_afEntry[11] * fA1;
	kInv( 2 , 3 ) = -m_afEntry[8] * fA4 + m_afEntry[9] * fA2 - m_afEntry[11] * fA0;
	kInv( 3 , 3 ) = +m_afEntry[8] * fA3 - m_afEntry[9] * fA1 + m_afEntry[10] * fA0;

	float fDet = fA0 * fB5 - fA1 * fB4 + fA2 * fB3 + fA3 * fB2 - fA4 * fB1 + fA5 * fB0;

	float fInvDet = ( ( float )1.0f ) / fDet;
	for( int iRow = 0; iRow < 4; iRow++ )
	{
		for( int iCol = 0; iCol < 4; iCol++ )
		{
			kInv( iRow , iCol ) *= fInvDet;
		}
	}

	return kInv;
}

SVector3f SMatrix4f::GetBasisX() const
{
	SVector3f Basis;

	for( int i = 0; i < 3; i++ )
	{
		Basis[i] = m_afEntry[I( 0 , i )];
	}

	return Basis;
}

SVector3f SMatrix4f::GetBasisY() const
{
	SVector3f Basis;

	for( int i = 0; i < 3; i++ )
	{
		Basis[i] = m_afEntry[I( 1 , i )];
	}

	return Basis;
}

SVector3f SMatrix4f::GetBasisZ() const
{
	SVector3f Basis;

	for( int i = 0; i < 3; i++ )
	{
		Basis[i] = m_afEntry[I( 2 , i )];
	}

	return Basis;
}

SVector3f SMatrix4f::GetTranslate() const
{
	SVector3f Pos;

	for( int i = 0; i < 3; i++ )
	{
		Pos[i] = m_afEntry[I( 3 , i )];
	}

	return Pos;
}

void SMatrix4f::SetTranslate( const SVector3f& Trans )
{
	for( int i = 0; i < 3; i++ )
	{
		m_afEntry[I( 3 , i )] = Trans[i];
	}
}

SMatrix3f SMatrix4f::GetRotation() const
{
	SMatrix3f mRet;

	mRet.SetRow( 0 , GetBasisX() );
	mRet.SetRow( 1 , GetBasisY() );
	mRet.SetRow( 2 , GetBasisZ() );

	return mRet;
}

void SMatrix4f::SetRotation( const SMatrix3f& Rot )
{
	for( int iRow = 0; iRow < 3; iRow++ )
	{
		for( int iCol = 0; iCol < 3; iCol++ )
		{
			m_afEntry[I( iRow , iCol )] = Rot( iRow , iCol );
		}
	}
}

SMatrix4f SMatrix4f::RotationMatrixXYZ( float fRadiansX , float fRadiansY , float fRadiansZ )
{
	return SMatrix4f::RotationMatrixZ( fRadiansZ ) * SMatrix4f::RotationMatrixY( fRadiansY ) * SMatrix4f::RotationMatrixX( fRadiansX );
}

SMatrix4f SMatrix4f::RotationMatrixX( float fRadians )
{
	SMatrix4f ret;

	ret.RotationX( fRadians );

	return ret;
}

SMatrix4f SMatrix4f::RotationMatrixY( float fRadians )
{
	SMatrix4f ret;
	ret.RotationY( fRadians );
	return ret;
}

SMatrix4f SMatrix4f::RotationMatrixZ( float fRadians )
{
	SMatrix4f ret;
	ret.RotationZ( fRadians );
	return ret;
}

SMatrix4f SMatrix4f::ScaleMatrix( const SVector3f& scale )
{
	SMatrix4f ret;

	ret.m_afEntry[0] = scale.x;		ret.m_afEntry[1] = 0.0f;	ret.m_afEntry[2] = 0.0f;		ret.m_afEntry[3] = 0.0f;
	ret.m_afEntry[4] = 0.0f;		ret.m_afEntry[5] = scale.y;	ret.m_afEntry[6] = 0.0f;		ret.m_afEntry[7] = 0.0f;
	ret.m_afEntry[8] = 0.0f;		ret.m_afEntry[9] = 0.0f;	ret.m_afEntry[10] = scale.z;	ret.m_afEntry[11] = 0.0f;
	ret.m_afEntry[12] = 0.0f;		ret.m_afEntry[13] = 0.0f;	ret.m_afEntry[14] = 0.0f;		ret.m_afEntry[15] = 1.0f;

	return ret;
}

SMatrix4f SMatrix4f::ScaleMatrix( float fScale )
{
	SMatrix4f ret;
	ret.Scale( fScale );
	return ret;
}

SMatrix4f SMatrix4f::ScaleMatrixXYZ( float fX , float fY , float fZ )
{
	SMatrix4f ret;

	ret.m_afEntry[0] = fX;		ret.m_afEntry[1] = 0.0f;	ret.m_afEntry[2] = 0.0f;	ret.m_afEntry[3] = 0.0f;
	ret.m_afEntry[4] = 0.0f;	ret.m_afEntry[5] = fY;		ret.m_afEntry[6] = 0.0f;	ret.m_afEntry[7] = 0.0f;
	ret.m_afEntry[8] = 0.0f;	ret.m_afEntry[9] = 0.0f;	ret.m_afEntry[10] = fZ;		ret.m_afEntry[11] = 0.0f;
	ret.m_afEntry[12] = 0.0f;	ret.m_afEntry[13] = 0.0f;	ret.m_afEntry[14] = 0.0f;	ret.m_afEntry[15] = 1.0f;

	return ret;
}

SMatrix4f SMatrix4f::TranslationMatrix( float fX , float fY , float fZ )
{
	SMatrix4f ret;
	ret.Translate( fX , fY , fZ );
	return ret;
}

SMatrix4f SMatrix4f::LookAtLHMatrix( const SVector3f& eye , const SVector3f& at )
{
	/*
		translate:
		|1		0		0		0|
		|0		1		0		0|
		|0		0		1		0|
		|-cam_x	-cam_y	-cam_z	1|
		rotate:
		|right.x	up.x	look.x		0|
		|right.y	up.y	look.y		0|
		|right.z	up.z	look.z		0|
		|0			0		0			1|


		worldToCamera
		|1		0		0		0|			|right.x	up.x	look.x		0|
		|0		1		0		0|			|right.y	up.y	look.y		0|
		|0		0		1		0|	*		|right.z	up.z	look.z		0|
		|-cam_x	-cam_y	-cam_z	1|			|0			0		0			1|
		=
		|right.x			up.x			look.x			0|
		|right.y			up.y			look.y			0|
		|right.z			up.z			look.z			0|
		|-cam * right		-cam * up		-cam * look		1|
	*/
	SMatrix4f ret;

	SVector3f zAxis = at - eye;
	zAxis.Normalize();

	SVector3f up = zAxis.y < 0.999f ? SVector3f(0.0f, 1.0f, 0.0f) : SVector3f(0.0f, 0.0f, -1.0f);

	SVector3f xAxis = SVector3f::cross( up , zAxis );
	xAxis.Normalize();
	
	SVector3f yAxis = SVector3f::cross( zAxis , xAxis );
	yAxis.Normalize();

	ret.m_afEntry[0] = xAxis.x;		ret.m_afEntry[1] = yAxis.x;		ret.m_afEntry[2] = zAxis.x;		ret.m_afEntry[3] = 0.0f;
	ret.m_afEntry[4] = xAxis.y;		ret.m_afEntry[5] = yAxis.y;		ret.m_afEntry[6] = zAxis.y;		ret.m_afEntry[7] = 0.0f;
	ret.m_afEntry[8] = xAxis.z;		ret.m_afEntry[9] = yAxis.z;		ret.m_afEntry[10] = zAxis.z;	ret.m_afEntry[11] = 0.0f;

	ret.m_afEntry[12] = -( dot( xAxis , eye ) );

	ret.m_afEntry[13] = -( dot( yAxis , eye ) );

	ret.m_afEntry[14] = -( dot( zAxis , eye ) );

	ret.m_afEntry[15] = 1.0f;

	return ret;
}

SMatrix4f SMatrix4f::PerspectiveFovLHMatrix( float fovy , float aspect , float zn , float zf )
{
	SMatrix4f ret;

	float tanY = tan( fovy / 2.0f );
	if( 0.0f == tanY )	tanY = 0.001f;
	float yScale = 1.0f / tanY;

	if( 0.0f == aspect )	aspect = 0.001f;
	float xScale = yScale / aspect;

	float range = zf / ( zf - zn );

	ret.m_afEntry[0] = xScale;		ret.m_afEntry[1] = 0.0f;	ret.m_afEntry[2] = 0.0f;					ret.m_afEntry[3] = 0.0f;
	ret.m_afEntry[4] = 0.0f;		ret.m_afEntry[5] = yScale;	ret.m_afEntry[6] = 0.0f;					ret.m_afEntry[7] = 0.0f;
	ret.m_afEntry[8] = 0.0f;		ret.m_afEntry[9] = 0.0f;	ret.m_afEntry[10] = range;					ret.m_afEntry[11] = 1.0f;
	ret.m_afEntry[12] = 0.0f;		ret.m_afEntry[13] = 0.0f;	ret.m_afEntry[14] = -range * zn;			ret.m_afEntry[15] = 0.0f;

	return ret;
}

SMatrix4f SMatrix4f::OrthographicLHMatrix( float zn , float zf , float width , float height )
{
	SMatrix4f ret;

	if( zn == zf )	zf = zn + 0.1f;

	if( width <= 0.0f )		width = 1.0f;
	if( height <= 0.0f )	height = 1.0f;

	ret.m_afEntry[0] = 2.0f / width;	ret.m_afEntry[1] = 0.0f;			ret.m_afEntry[2] = 0.0f;					ret.m_afEntry[3] = 0.0f;
	ret.m_afEntry[4] = 0.0f;			ret.m_afEntry[5] = 2.0f / height;	ret.m_afEntry[6] = 0.0f;					ret.m_afEntry[7] = 0.0f;
	ret.m_afEntry[8] = 0.0f;			ret.m_afEntry[9] = 0.0f;			ret.m_afEntry[10] = 1.0f / ( zf - zn );		ret.m_afEntry[11] = 0.0f;
	ret.m_afEntry[12] = 0.0f;			ret.m_afEntry[13] = 0.0f;			ret.m_afEntry[14] = zn / ( zn - zf );		ret.m_afEntry[15] = 1.0f;

	return ret;
}

float SMatrix4f::operator[] ( int iPos ) const
{
	return m_afEntry[iPos];
}

float& SMatrix4f::operator[] ( int iPos )
{
	return ( m_afEntry[iPos] );
}

float SMatrix4f::operator() ( int iRow , int iCol ) const
{
	return ( m_afEntry[I( iRow , iCol )] );
}

float& SMatrix4f::operator() ( int iRow , int iCol )
{
	return m_afEntry[I( iRow , iCol )];
}

bool SMatrix4f::operator== ( const SMatrix4f& Matrix ) const
{
	return ( memcmp( m_afEntry , Matrix.m_afEntry , 4 * 4 * sizeof( float ) ) == 0 );
}

bool SMatrix4f::operator!= ( const SMatrix4f& Matrix ) const
{
	return ( memcmp( m_afEntry , Matrix.m_afEntry , 4 * 4 * sizeof( float ) ) != 0 );
}

SMatrix4f SMatrix4f::operator* ( const SMatrix4f& Matrix ) const
{
	SMatrix4f prod;

	for( int iRow = 0; iRow < 4; iRow++ )
	{
		for( int iCol = 0; iCol < 4; iCol++ )
		{
			int i = I( iRow , iCol );
			prod.m_afEntry[i] = 0.0f;
			for( int iMid = 0; iMid < 4; iMid++ )
			{
				prod.m_afEntry[i] += m_afEntry[I( iRow , iMid )] * Matrix.m_afEntry[I( iMid , iCol )];
			}
		}
	}

	return prod;
}

SMatrix4f SMatrix4f::operator+ ( const SMatrix4f& Matrix ) const
{
	SMatrix4f sum;

	for( int i = 0; i < 4 * 4; i++ )
	{
		sum.m_afEntry[i] = m_afEntry[i] + Matrix.m_afEntry[i];
	}

	return sum;
}

SMatrix4f SMatrix4f::operator- ( const SMatrix4f& Matrix ) const
{
	SMatrix4f diff;

	for( int i = 0; i < 4 * 4; i++ )
	{
		diff.m_afEntry[i] = m_afEntry[i] - Matrix.m_afEntry[i];
	}

	return diff;
}

SMatrix4f SMatrix4f::operator* ( float fScalar ) const
{
	SMatrix4f prod;

	for( int i = 0; i < 4 * 4; i++ )
	{
		prod.m_afEntry[i] = m_afEntry[i] * fScalar;
	}

	return prod;
}

SMatrix4f SMatrix4f::operator/ ( float fScalar ) const
{
	SMatrix4f quot;
	if( fScalar != 0.0f )
	{
		float fInvScalar = 1.0f / fScalar;

		for( int i = 0; i < 4 * 4; i++ )
		{
			quot.m_afEntry[i] = m_afEntry[i] * fInvScalar;
		}
	}
	else
	{
		for( int i = 0; i < 4 * 4; i++ )
		{
			quot.m_afEntry[i] = 0.0f;
		}
	}

	return quot;
}

SMatrix4f SMatrix4f::operator- ( ) const
{
	SMatrix4f neq;

	for( int i = 0; i < 4 * 4; i++ )
	{
		neq.m_afEntry[i] = -m_afEntry[i];
	}

	return neq;
}

SMatrix4f& SMatrix4f::operator+= ( const SMatrix4f& Matrix )
{
	for( int i = 0; i < 4 * 4; i++ )
	{
		m_afEntry[i] += Matrix.m_afEntry[i];
	}

	return ( *this );
}

SMatrix4f& SMatrix4f::operator-= ( const SMatrix4f& Matrix )
{
	for( int i = 0; i < 4 * 4; i++ )
	{
		m_afEntry[i] -= Matrix.m_afEntry[i];
	}

	return ( *this );
}

SMatrix4f& SMatrix4f::operator*= ( float fScalar )
{
	for( int i = 0; i < 4 * 4; i++ )
	{
		m_afEntry[i] *= fScalar;
	}

	return ( *this );
}

SMatrix4f& SMatrix4f::operator*= ( const SMatrix4f& Matrix )
{
	SMatrix4f mProd = *this;

	for( int iRow = 0; iRow < 4; iRow++ )
	{
		for( int iCol = 0; iCol < 4; iCol++ )
		{
			int i = I( iRow , iCol );
			m_afEntry[i] = 0.0f;
			for( int iMid = 0; iMid < 4; iMid++ )
			{
				m_afEntry[i] += mProd.m_afEntry[I( iRow , iMid )] * Matrix.m_afEntry[I( iMid , iCol )];
			}
		}
	}

	return ( *this );
}

SMatrix4f& SMatrix4f::operator/= ( float fScalar )
{
	if( fScalar != 0.0f )
	{
		float fInvScalar = 1.0f / fScalar;

		for( int i = 0; i < 4 * 4; i++ )
		{
			m_afEntry[i] *= fInvScalar;
		}
	}
	else
	{
		for( int i = 0; i < 4 * 4; i++ )
		{
			m_afEntry[i] = 0.0f;
		}
	}

	return ( *this );
}

void SMatrix4f::MakeZero()
{
	memset( m_afEntry , 0 , 4 * 4 * sizeof( float ) );
}

void SMatrix4f::MakeIdentity()
{
	for( int iRow = 0; iRow < 4; iRow++ )
	{
		for( int iCol = 0; iCol < 4; iCol++ )
		{
			if( iRow == iCol )
			{
				m_afEntry[I( iRow , iCol )] = 1.0f;
			}
			else
			{
				m_afEntry[I( iRow , iCol )] = 0.0f;
			}
		}
	}
}

void SMatrix4f::MakeTranspose()
{
	SMatrix4f Trans;

	for( int iRow = 0; iRow < 4; iRow++ )
	{
		for( int iCol = 0; iCol < 4; iCol++ )
		{
			Trans.m_afEntry[I( iRow , iCol )] = m_afEntry[I( iCol , iRow )];
		}
	}

	memcpy( m_afEntry , Trans.m_afEntry , 4 * 4 * sizeof( float ) );
}

SMatrix4f SMatrix4f::Zero()
{
	SMatrix4f zero = SMatrix4f( true );

	return zero;
}

SMatrix4f SMatrix4f::Identity()
{
	SMatrix4f ident;

	for( int iRow = 0; iRow < 4; iRow++ )
	{
		for( int iCol = 0; iCol < 4; iCol++ )
		{
			if( iRow == iCol )
			{
				ident.m_afEntry[I( iRow , iCol )] = 1.0f;
			}
			else
			{
				ident.m_afEntry[I( iRow , iCol )] = 0.0f;
			}
		}
	}

	return ident;
}

SMatrix4f SMatrix4f::Transpose()
{
	SMatrix4f mTranspose;

	for( int iRow = 0; iRow < 4; iRow++ )
	{
		for( int iCol = 0; iCol < 4; iCol++ )
		{
			mTranspose.m_afEntry[I( iRow , iCol )] = m_afEntry[I( iCol , iRow )];
		}
	}

	return mTranspose;
}

int SMatrix4f::I( int iRow , int iCol )
{
	return ( 4 * iRow + iCol );
}

SVector4f SMatrix4f::operator* ( const SVector4f& Vector ) const
{
	SVector4f Prod;

	for( int iRow = 0; iRow < 4; iRow++ )
	{
		Prod[iRow] = 0.0f;
		for( int iCol = 0; iCol < 4; iCol++ )
		{
			Prod[iRow] += m_afEntry[I( iRow , iCol )] * Vector[iCol];
		}
	}

	return Prod;
}

void SMatrix4f::SetRow( int iRow , const SVector4f& Vector )
{
	for( int iCol = 0; iCol < 4; iCol++ )
	{
		m_afEntry[I( iRow , iCol )] = Vector[iCol];
	}
}

void SMatrix4f::SetRow( int iRow , const SVector3f& Vector )
{
	for ( int iCol = 0; iCol < 3; iCol++ )
	{
		m_afEntry[I( iRow , iCol )] = Vector[iCol];
	}
}

void SMatrix4f::SetColumn( int iCol , const SVector4f& Vector )
{
	for( int iRow = 0; iRow < 4; iRow++ )
	{
		m_afEntry[I( iRow , iCol )] = Vector[iRow];
	}
}

SVector4f SMatrix4f::GetRow( int iRow ) const
{
	SVector4f Row;

	for( int iCol = 0; iCol < 4; iCol++ )
	{
		Row[iCol] = m_afEntry[I( iRow , iCol )];
	}

	return Row;
}

SVector4f SMatrix4f::GetColumn( int iCol ) const
{
	SVector4f Col;

	for( int iRow = 0; iRow < 4; iRow++ )
	{
		Col[iRow] = m_afEntry[I( iRow , iCol )];
	}

	return Col;
}