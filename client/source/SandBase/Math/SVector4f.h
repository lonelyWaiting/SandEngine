#ifndef Vector4f_h
#define Vector4f_h

#include "SVector3f.h"
#include <cmath>

class SVector2f;

class SVector4f
{
public:
	inline SVector4f()
		:x( 0.0f ) , y( 0.0f ) , z( 0.0f ) , w( 0.0f )
	{

	}

	inline SVector4f( float _x , float _y , float _z , float _w )
		: x( _x ) , y( _y ) , z( _z ) , w( _w )
	{

	}

	inline SVector4f( const float rhs[4] )
		: x( rhs[0] ) , y( rhs[1] ) , z( rhs[2] ) , w( rhs[3] )
	{

	}

	inline SVector4f( const SVector3f& rhs )
		: x( rhs.x ) , y( rhs.y ) , z( rhs.z ) , w( 1.0f )
	{

	}

	inline SVector4f( const SVector4f& rhs )
		: x( rhs.x ) , y( rhs.y ) , z( rhs.z ) , w( rhs.w )
	{

	}

	static SVector4f Zero()
	{
		return SVector4f( 0.0f , 0.0f , 0.0f , 0.0f );
	}
public:
	inline SVector4f& operator= ( const SVector4f& rhs )
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;

		return *this;
	}

	inline SVector4f operator+ ( const SVector4f& rhs ) const
	{
		return SVector4f( x + rhs.x , y + rhs.y , z + rhs.z , w + rhs.w );
	}

	inline SVector4f operator- ( const SVector4f& rhs ) const
	{
		return SVector4f( x - rhs.x , y - rhs.y , z - rhs.z , w - rhs.w );
	}

	inline SVector4f operator* ( const float scale ) const
	{
		return SVector4f( x * scale , y * scale , z * scale , w * scale );
	}

	inline SVector4f operator* ( const SVector4f& rhs ) const
	{
		return SVector4f( x * rhs.x , y * rhs.y , z * rhs.z , w * rhs.w );
	}

	inline SVector4f operator- () const
	{
		return SVector4f( -x , -y , -z , -w );
	}

	inline SVector4f& operator+= ( const SVector4f& rhs )
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;

		return *this;
	}

	inline SVector4f& operator-= ( const SVector4f& rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;

		return *this;
	}

	inline SVector4f& operator*= ( const float scale )
	{
		x *= scale;
		y *= scale;
		z *= scale;
		w *= scale;

		return *this;
	}

	inline SVector4f& operator*= ( const SVector4f& rhs )
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;

		return *this;
	}

	inline friend SVector4f operator* ( const float lhs , const SVector4f& rhs )
	{
		return SVector4f( lhs * rhs.x , lhs * rhs.y , lhs * rhs.z , lhs * rhs.w );
	}

	inline bool operator== ( const SVector4f& Vector ) const
	{
		if( ( x - Vector.x ) * ( x - Vector.x ) > 0.01f )
		{
			return false;
		}
		if( ( y - Vector.y ) * ( y - Vector.y ) > 0.01f )
		{
			return false;
		}
		if( ( z - Vector.z ) * ( z - Vector.z ) > 0.01f )
		{
			return false;
		}
		if( ( w - Vector.w ) * ( w - Vector.w ) > 0.01f )
		{
			return false;
		}

		return true;
	}

	inline bool operator!= ( const SVector4f& Vector ) const
	{
		return ( !( *this == Vector ) );
	}

	inline float operator[] ( int index ) const
	{
		assert( index < 4 );

		return m[index];
	}

	inline float& operator[] ( int index )
	{
		assert( index < 4 );

		return m[index];
	}

	inline void clamp( float min , float max )
	{
		if( min > max ) { float temp = max; max = min; min = temp; }

		if( x > max )	x = max;
		if( x < min )	x = min;

		if( y > max )	y = max;
		if( y < min )	y = min;

		if( z > max )	z = max;
		if( z < min )	z = min;

		if( w > max )	w = max;
		if( w < min )	w = min;
	}

public:
	inline friend float dot( const SVector4f& lhs , const SVector4f& rhs )
	{
		return ( ( lhs.x * rhs.x ) + ( lhs.y * rhs.y ) + ( lhs.z * rhs.z ) + ( lhs.w * rhs.w ) );
	}

	inline void makeZero()
	{
		x = y = z = w = 0.0f;
	}

	inline void Normalize()
	{
		float fInvMag = ( 1.0f / Magnitude() );

		x *= fInvMag;
		y *= fInvMag;
		z *= fInvMag;
		w *= fInvMag;
	}

	inline float Magnitude()
	{
		return sqrt( ( x * x ) + ( y * y ) + ( z * z ) + ( w * w ) );
	}

	SVector4f operator*( const class SMatrix4f& mat );

public:
	SVector3f xyz() const;
	SVector2f xy() const;

public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		float m[4];
	};
};
#endif