#ifndef Vector3f_h
#define Vector3f_h

#include <cassert>

class SVector3f
{
public:
	inline SVector3f()
		:x( 0 ) , y( 0 ) , z( 0 )
	{

	}

	inline SVector3f( float _x , float _y , float _z )
		: x( _x ) , y( _y ) , z( _z )
	{

	}

	inline SVector3f( const SVector3f& rhs )
		: x( rhs.x ) , y( rhs.y ) , z( rhs.z )
	{

	}

	inline SVector3f( const float rhs[3] )
		: x( rhs[0] ) , y( rhs[1] ) , z( rhs[2] )
	{

	}

	static SVector3f Zero()
	{
		return SVector3f( 0.0f , 0.0f , 0.0f );
	}

public:
	inline SVector3f& operator= ( const SVector3f& rhs )
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;

		return *this;
	}

	inline SVector3f operator+ ( const SVector3f& rhs ) const
	{
		return SVector3f( x + rhs.x , y + rhs.y , z + rhs.z );
	}

	inline SVector3f operator- ( const SVector3f& rhs ) const
	{
		return SVector3f( x - rhs.x , y - rhs.y , z - rhs.z );
	}

	inline SVector3f operator* ( const float scale ) const
	{
		return SVector3f( x * scale , y * scale , z * scale );
	}

	inline SVector3f operator* ( const SVector3f& rhs ) const
	{
		return SVector3f( x * rhs.x , y * rhs.y , z * rhs.z );
	}

	inline SVector3f operator- () const
	{
		return SVector3f( -x , -y , -z );
	}

	inline SVector3f& operator+= ( const SVector3f& rhs )
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}

	inline SVector3f& operator-= ( const SVector3f& rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}

	inline SVector3f& operator*= ( const float scale )
	{
		x *= scale;
		y *= scale;
		z *= scale;

		return *this;
	}

	inline SVector3f& operator*= ( const SVector3f& rhs )
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;

		return *this;
	}

	inline friend SVector3f operator* ( const float lhs , const SVector3f& rhs )
	{
		return SVector3f( lhs * rhs.x , lhs * rhs.y , lhs * rhs.z );
	}

	inline float operator[] ( int index ) const
	{
		assert( index < 3 );

		return m[index];
	}

	inline float& operator[] ( int index )
	{
		assert( index < 3 );

		return m[index];
	}

	inline bool operator== ( const SVector3f& Vector ) const
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

		return true;
	}

	inline bool operator!= ( const SVector3f& Vector ) const
	{
		return ( !( *this == Vector ) );
	}

public:
	inline friend float dot( const SVector3f& lhs , const SVector3f& rhs )
	{
		return ( lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z );
	}

	inline friend SVector3f cross( const SVector3f& lhs , const SVector3f& rhs )
	{
		return SVector3f( lhs.y * rhs.z - lhs.z * rhs.y ,
						 lhs.z * rhs.x - lhs.x * rhs.z ,
						 lhs.x * rhs.y - lhs.y * rhs.x );
	}

	inline void clamp( float min , float max )
	{
		if( min > max ) { float temp = max; max = min; min = temp; }

		if( x < min )	x = min;
		if( x > max )	x = max;

		if( y < min )	y = min;
		if( y > max )	y = max;
		
		if( z < min )	z = min;
		if( z > max )	z = max;
	}

	inline void makeZero()
	{
		x = y = z = 0.0f;
	}

	SVector3f operator* ( const class SMatrix3f& mat );

public:
	static SVector3f Normalize( const SVector3f& v );

	void Normalize();
	float Magnitude() const;
	SVector3f Perpendicular();

	static SVector3f Random();
	static SVector3f Normalize( SVector3f& A );

public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		float m[3];
	};
};
#endif