#ifndef MATRIX4F_H
#define MATRIX4F_H

class SVector4f;
class SVector3f;
class SMatrix3f;

class SMatrix4f
{
public:
	SMatrix4f();
	SMatrix4f( bool bZero );
	SMatrix4f( const SMatrix4f& Matrix );
	SMatrix4f( float fM11 , float fM12 , float fM13 , float fM14 ,
			   float fM21 , float fM22 , float fM23 , float fM24 ,
			   float fM31 , float fM32 , float fM33 , float fM34 ,
			   float fM41 , float fM42 , float fM43 , float fM44 );
	SMatrix4f Inverse() const;

	void RotationX( float fRadians );
	void RotationY( float fRadians );
	void RotationZ( float fRadians );
	void Scale( float fScale );
	void Translate( float fX , float fY , float fZ );

	SVector3f GetBasisX() const;
	SVector3f GetBasisY() const;
	SVector3f GetBasisZ() const;

	SVector3f GetTranslate() const;
	SMatrix3f GetRotation() const;

	void SetRotation( const SMatrix3f& Rot );
	void SetTranslate( const SVector3f& Trans );

	static SMatrix4f RotationMatrixXYZ( float fRadiansX , float fRadiansY , float fRadiansZ );
	static SMatrix4f RotationMatrixX( float fRadians );
	static SMatrix4f RotationMatrixY( float fRadians );
	static SMatrix4f RotationMatrixZ( float fRadians );
	static SMatrix4f ScaleMatrix( float fScale );
	static SMatrix4f ScaleMatrix( const SVector3f& scale );
	static SMatrix4f ScaleMatrixXYZ( float fX , float fY , float fZ );
	static SMatrix4f TranslationMatrix( float fX , float fY , float fZ );
	static SMatrix4f LookAtLHMatrix( const SVector3f& eye , const SVector3f& at );
	static SMatrix4f PerspectiveFovLHMatrix( float fovy , float aspect , float zn , float zf );
	static SMatrix4f OrthographicLHMatrix( float zn , float zf , float width , float height );

	void MakeZero();
	void MakeIdentity();
	void MakeTranspose();

	static SMatrix4f Zero();
	static SMatrix4f Identity();
	SMatrix4f Transpose();

	SMatrix4f& operator= ( const SMatrix4f& Matrix );

	float operator() ( int iRow , int iCol ) const;
	float& operator() ( int iRow , int iCol );
	float operator[] ( int iPos ) const;
	float& operator[] ( int iPos );

	void SetRow( int iRow , const SVector4f& Vector );
	void SetRow( int iRow , const SVector3f& Vector );
	SVector4f GetRow( int iRow ) const;
	void SetColumn( int iCol , const SVector4f& Vector );
	SVector4f GetColumn( int iCol ) const;

	bool operator== ( const SMatrix4f& Matrix ) const;
	bool operator!= ( const SMatrix4f& Matrix ) const;

	SMatrix4f operator+ ( const SMatrix4f& Matrix ) const;
	SMatrix4f operator- ( const SMatrix4f& Matrix ) const;
	SMatrix4f operator* ( const SMatrix4f& Matrix ) const;
	SMatrix4f operator* ( float fScalar ) const;
	SMatrix4f operator/ ( float fScalar ) const;
	SMatrix4f operator- () const;

	SMatrix4f& operator+= ( const SMatrix4f& Matrix );
	SMatrix4f& operator-= ( const SMatrix4f& Matrix );
	SMatrix4f& operator*= ( const SMatrix4f& Matrix );
	SMatrix4f& operator*= ( float fScalar );
	SMatrix4f& operator/= ( float fScalar );

	SVector4f operator* ( const SVector4f& V ) const;

	static const int m11 = 0;
	static const int m12 = 1;
	static const int m13 = 2;
	static const int m14 = 3;

	static const int m21 = 4;
	static const int m22 = 5;
	static const int m23 = 6;
	static const int m24 = 7;

	static const int m31 = 8;
	static const int m32 = 9;
	static const int m33 = 10;
	static const int m34 = 11;

	static const int m41 = 12;
	static const int m42 = 13;
	static const int m43 = 14;
	static const int m44 = 15;

protected:
	float m_afEntry[4 * 4];

	static int I( int iRow , int iCol );
};

#endif