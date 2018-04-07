#ifndef Matrix3f_h
#define Matrix3f_h

#include "SVector3f.h"

class SMatrix3f
{
public:
	SMatrix3f();
	SMatrix3f( bool bZero );
	SMatrix3f( const SMatrix3f& Matrix );
	SMatrix3f( float fM11 , float fM12 , float fM13 ,
			   float fM21 , float fM22 , float fM23 ,
			   float fM31 , float fM32 , float fM33 );

	void RotationX( float fRadians );
	void RotationY( float fRadians );
	void RotationZ( float fRadians );

	void Rotation( SVector3f& Rot );
	void RotationZYX( SVector3f& Rot );
	void RotationEuler( SVector3f& Axis , float Angle );
	void Orthonormalize();

	void MakeZero();
	void MakeIdentity();
	void MakeTranspose();

	SMatrix3f Zero();
	SMatrix3f Identity();
	SMatrix3f Transpose();

	SMatrix3f& operator= ( const SMatrix3f& Matrix );

	float operator() ( int iRow , int iCol ) const;
	float& operator() ( int iRow , int iCol );
	float operator[] ( int iPos ) const;
	float& operator[] ( int iPos );

	void SetRow( int iRow , const SVector3f& Vector );
	SVector3f GetRow( int iRow ) const;
	void SetColumn( int iCol , const SVector3f& Vector );
	SVector3f GetColumn( int iCol ) const;

	bool operator== ( const SMatrix3f& Matrix ) const;
	bool operator!= ( const SMatrix3f& Matrix ) const;

	SMatrix3f operator+ ( const SMatrix3f& Matrix ) const;
	SMatrix3f operator- ( const SMatrix3f& Matrix ) const;
	SMatrix3f operator* ( const SMatrix3f& Matrix ) const;
	SMatrix3f operator* ( float fScalar ) const;
	SMatrix3f operator/ ( float fScalar ) const;
	SMatrix3f operator- () const;

	SMatrix3f& operator+= ( const SMatrix3f& Matrix );
	SMatrix3f& operator-= ( const SMatrix3f& Matrix );
	SMatrix3f& operator*= ( const SMatrix3f& Maatrix );
	SMatrix3f& operator*= ( float fScalar );
	SMatrix3f& operator/= ( float fScalar );

	SVector3f operator* ( const SVector3f& rKV ) const;

	friend SVector3f operator* ( const SVector3f& v , const SMatrix3f& matrix )
	{
		return SVector3f( v.x * matrix.m_afEntry[0] + v.y * matrix.m_afEntry[3] + v.z * matrix.m_afEntry[6] ,
						 v.x * matrix.m_afEntry[1] + v.y * matrix.m_afEntry[4] + v.z * matrix.m_afEntry[7] ,
						 v.x * matrix.m_afEntry[2] + v.y * matrix.m_afEntry[5] + v.z * matrix.m_afEntry[8] );
	}

	// 绕着轴Axis,旋转angle角度的旋转矩阵
	void RotateAxis( SVector3f& Axis , float angle );

protected:
	float m_afEntry[3 * 3];		// array float => af

	static int I( int iRow , int iCol );	// iRow * N + iCol
};
#endif