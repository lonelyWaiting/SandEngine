#include "SandBase/SandBasePCH.h"
#include "SQuaternion.h"
#include "SVector3f.h"
#include <cmath>

SQuaternion::SQuaternion()
{
	x = y = z = 0.0f;
	w = 1.0f;
}

SQuaternion::SQuaternion( float _x , float _y , float _z , float _w )
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

SQuaternion::SQuaternion( const SQuaternion & q )
{
	x = q.x;	y = q.y;	z = q.z;	w = q.w;
}

SQuaternion::SQuaternion( const SVector3f & axis , float theta )
{
	SVector3f normalizeAxis = SVector3f::Normalize( axis );

	float sinTheta = sinf( theta * 0.5f );
	float cosTheta = cosf( theta * 0.5f );

	x = sinTheta * normalizeAxis.x;
	y = sinTheta * normalizeAxis.y;
	z = sinTheta * normalizeAxis.z;
	w = cosTheta;
}

void SQuaternion::operator=( const SQuaternion& q )
{
	x = q.x;	y = q.y;	z = q.z;	w = q.w;
}

void SQuaternion::SetToRotateAboutX( float theta )
{
	float thetaOver2 = theta * 0.5f;

	w = cos( thetaOver2 );
	x = sin( thetaOver2 );
	y = z = 0.0f;
}

void SQuaternion::SetToRotateAboutY( float theta )
{
	float thetaOver2 = theta * 0.5f;

	w = cos( thetaOver2 );
	y = sin( thetaOver2 );
	x = z = 0.0f;
}

void SQuaternion::SetToRotateAboutZ( float theta )
{
	float thetaOver2 = theta * 0.5f;

	w = cos( thetaOver2 );
	z = sin( thetaOver2 );
	x = y = 0.0f;
}

void SQuaternion::SetToRotaeAboutAxis( const SVector3f & axis , float theta )
{
	float thetaOver2 = theta * 0.5f;
	float sinThetaOver2 = sin( thetaOver2 );

	w = cos( thetaOver2 );
	y = sinThetaOver2 * axis.x;
	x = sinThetaOver2 * axis.y;
	z = sinThetaOver2 * axis.z;
}

void SQuaternion::SetIdentity()
{
	x = y = z = 0.0f;
	w = 1.0f;
}

SQuaternion SQuaternion::operator*( const SQuaternion & a ) const
{
	SQuaternion result;

	result.w = w * a.w - x * a.x - y * a.y - z * a.z;
	result.x = w * a.x + x * a.w + z * a.y - y * a.z;
	result.y = w * a.y + y * a.w + x * a.z - z * a.x;
	result.z = w * a.z + z * a.w + y * a.x - x * a.y;

	return result;
}

SQuaternion & SQuaternion::operator*=( const SQuaternion & a )
{
	float rw = w * a.w - x * a.x - y * a.y - z * a.z;
	float rx = w * a.x + x * a.w + z * a.y - y * a.z;
	float ry = w * a.y + y * a.w + x * a.z - z * a.x;
	float rz = w * a.z + z * a.w + y * a.x - x * a.y;

	x = rx; y = ry; z = rz; w = rw;

	return *this;
}

void SQuaternion::normalize()
{
	float mag = ( float )sqrt( x * x + y * y + z * z + w * w );

	if( mag > 0.0f )
	{
		float oneOverMag = 1.0f / mag;

		w *= oneOverMag;
		x *= oneOverMag;
		y *= oneOverMag;
		z *= oneOverMag;
	}
	else
	{
		assert( false );

		SetIdentity();
	}
}

float SQuaternion::GetTheta() const
{
	float theta_div_2 = acosf( w );

	return 2 * theta_div_2;
}

SVector3f SQuaternion::GetRotateAxis() const
{
	float sinThetaOver2Sq = 1.0f - w * w;

	if( sinThetaOver2Sq <= 0.0f )
	{
		return SVector3f( 1.0f , 0.0f , 0.0f );
	}

	float oneOverSinThetaOver2 = 1.0f / sqrt( sinThetaOver2Sq );

	return SVector3f( x * oneOverSinThetaOver2 , y * oneOverSinThetaOver2 , z * oneOverSinThetaOver2 );
}

float dot( const SQuaternion & a , const SQuaternion & b )
{
	return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
}

SQuaternion slerp( const SQuaternion & q0 , const SQuaternion & q1 , float t )
{
	if( t <= 0.0f )	return q0;
	if( t >= 1.0f )	return q1;

	float cosOmega = dot( q0 , q1 );

	// 如果点乘为负，使用-q1
	// 四元数q和-q1代表相同的旋转,但可能产生不同的slerp运算,我们要选择正确的一个以便用锐角进行旋转

	float q1w = q1.w;
	float q1x = q1.x;
	float q1y = q1.y;
	float q1z = q1.z;

	if( cosOmega < 0.0f )
	{
		q1w = -q1w;
		q1x = -q1x;
		q1y = -q1y;
		q1z = -q1z;

		cosOmega = -cosOmega;
	}

	float k0 , k1;
	if( cosOmega > 0.9999f )
	{
		// 非常接近，即线性插值，防止除零
		k0 = 1.0f - t;
		k1 = t;
	}
	else
	{
		float sinOmega = sqrt( 1.0f - cosOmega * cosOmega );
		float omega = atan2( sinOmega , cosOmega );
		float oneOverSinOmega = 1.0f / sinOmega;

		k0 = sin( ( 1.0f - t ) * omega ) * oneOverSinOmega;
		k1 = sin( t * omega ) * oneOverSinOmega;
	}

	SQuaternion result;
	result.x = k0 * q0.x + k1 * q1.x;
	result.y = k0 * q0.y + k1 * q1.y;
	result.z = k0 * q0.z + k1 * q1.z;
	result.w = k0 * q0.w + k1 * q1.w;

	return result;
}

SQuaternion conjugate( const SQuaternion & a )
{
	SQuaternion result;

	// 旋转量相同
	result.w = a.w;

	// 旋转轴相反
	result.x = -a.x;
	result.y = -a.y;
	result.z = -a.z;

	return result;
}

SQuaternion pow( const SQuaternion & q , float exponent )
{
	// 避免除0，cos为1,则sin为0
	if( fabs( q.w ) > 0.9999f )
	{
		return q;
	}

	float alpha    = acos( q.w );
	float newAlpha = alpha * exponent;
	float mult     = sin( newAlpha ) / sin( alpha );

	SQuaternion result;
	result.w = cos( newAlpha );
	result.x = q.x * mult;
	result.y = q.y * mult;
	result.z = q.z * mult;

	return result;
}

SVector3f rotate( const SVector3f & pos , const SQuaternion & q )
{
	SQuaternion temp;
	temp.x = pos.x;
	temp.y = pos.y;
	temp.z = pos.z;
	temp.w = 0.0f;

	SQuaternion result = q * temp * conjugate( q );

	return SVector3f( result.x , result.y , result.z );
}
