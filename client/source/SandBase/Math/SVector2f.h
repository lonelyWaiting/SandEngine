#ifndef Vector2f_h
#define Vector2f_h

#include <assert.h>

class SVector2f
{
public:
	inline SVector2f()
		:x( 0 ) , y( 0 )
	{

	}

	inline SVector2f( const float _x , const float _y )
		: x( _x ) , y( _y )
	{

	}

	inline SVector2f( const SVector2f& rhs )
		: x( rhs.x ) , y( rhs.y )
	{

	}

	inline SVector2f( const float vector[2] )
		: x( vector[0] ) , y( vector[1] )
	{

	}

public:
	inline SVector2f& operator= ( const SVector2f& rhs )
	{
		x = rhs.x;
		y = rhs.y;

		return *this;
	}

	inline SVector2f operator+ ( const SVector2f& rhs ) const
	{
		return SVector2f( x + rhs.x , y + rhs.y );
	}

	inline SVector2f operator- ( const SVector2f& rhs ) const
	{
		return SVector2f( x - rhs.x , y - rhs.y );
	}

	inline SVector2f operator* ( const float scale ) const
	{
		return SVector2f( x * scale , y * scale );
	}

	inline SVector2f operator* ( const SVector2f& rhs ) const
	{
		return SVector2f( x * rhs.x , y * rhs.y );
	}

	inline SVector2f operator- () const
	{
		return SVector2f( -x , -y );
	}

	inline SVector2f& operator+= ( const SVector2f& rhs )
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	inline SVector2f& operator-= ( const SVector2f& rhs )
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	inline SVector2f& operator*= ( const float scale )
	{
		x *= scale;
		y *= scale;

		return *this;
	}

	inline SVector2f& operator*= ( const SVector2f& rhs )
	{
		x *= rhs.x;
		y *= rhs.y;

		return *this;
	}

	inline friend SVector2f operator* ( const float lhs , const SVector2f& rhs )
	{
		return SVector2f( lhs * rhs.x , lhs * rhs.y );
	}

	inline float operator[] ( int index ) const
	{
		assert( index >= 0 && index < 2 );

		return m[index];
	}

	inline float& operator[] ( int index )
	{
		assert( index >= 0 && index < 2 );

		return m[index];
	}

	inline bool operator== ( const SVector2f& Vector ) const
	{
		if( ( x - Vector.x ) * ( x - Vector.x ) > 0.01f )
		{
			return false;
		}

		if( ( y - Vector.y ) * ( y - Vector.y ) > 0.01f )
		{
			return false;
		}

		return true;
	}

	inline bool operator!= ( const SVector2f& Vector ) const
	{
		return ( !( *this == Vector ) );
	}

	inline void clamp( float min , float max )
	{
		if( min > max ) { float temp = max; max = min; min = temp; }

		if( x > max )	x = max;
		if( x < min )	x = min;

		if( y > max )	y = max;
		if( y < min )	y = min;
	}

	inline void makeZero()
	{
		x = y = 0.0f;
	}

public:
	union
	{
		struct
		{
			float x;
			float y;
		};

		float m[2];
	};
};
#endif