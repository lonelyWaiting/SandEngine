#pragma once

class SVector3f;

class SQuaternion
{
public:
	SQuaternion();

	SQuaternion( float _x , float _y , float _z , float _w );

	SQuaternion( const SQuaternion& q );

	SQuaternion( const SVector3f& axis , float theta );

	void operator=( const SQuaternion& q );

	void SetToRotateAboutX( float theta );
	void SetToRotateAboutY( float theta );
	void SetToRotateAboutZ( float theta );
	void SetToRotaeAboutAxis( const SVector3f& axis , float theta );

	void SetIdentity();

	SQuaternion operator*( const SQuaternion& a ) const;
	SQuaternion& operator*=( const SQuaternion& a );

	void normalize();
	float GetTheta() const;
	SVector3f GetRotateAxis() const;

	friend float dot( const SQuaternion& a , const SQuaternion& b );
	friend SQuaternion slerp( const SQuaternion& a , const SQuaternion& b , float t );
	// 共轭四元数
	friend SQuaternion conjugate( const SQuaternion& q );
	friend SQuaternion pow( const SQuaternion& q , float exponent );
	friend SVector3f rotate( const SVector3f& pos , const SQuaternion& q );

public:
	union
	{
		struct 
		{
			float x , y , z , w;
		};
		struct  
		{
			float image[3];
			float real;
		};
	};
};