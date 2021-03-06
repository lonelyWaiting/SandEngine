#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "SandBase/String/SString.h"
#include "SandBase/Vector/SArray.h"

TEST( SandBaseTest , String )
{
	SString str;
	std::cout << str.AsChar() << std::endl;

	EXPECT_EQ( strlen( str.AsChar() ) , 0 );
	EXPECT_TRUE( str.IsEmpty() );

	str.Append( "test_string" );
	EXPECT_STREQ( str.AsChar() , "test_string" );

	str.AppendFormat( "_%d_%s" , 3 , "money" );
	EXPECT_STREQ( str.AsChar() , "test_string_3_money" );

	EXPECT_TRUE( str.StartWith( "test_" ) );;
	EXPECT_TRUE( str.EndWith( "money" ) );

	str = "neW_Test";
	EXPECT_TRUE( str.GetLength() == 8 );
	EXPECT_STREQ( str.AsChar() , "neW_Test" );
	EXPECT_TRUE( str.StartWith_NoCase( "new" ) );
	EXPECT_TRUE( str.EndWith_NoCase( "test" ) );

	str.toLower();
	EXPECT_STREQ( str.AsChar() , "new_test" );

	str.toUpper();
	EXPECT_STREQ( str.AsChar() , "NEW_TEST" );

	str = "BBC ABCDAB ABCDABCDABDE";
	EXPECT_EQ( str.Find( "ABCDABD" ) , 15 );
	EXPECT_EQ( str.Find_NoCase( "aBCdABD" ) , 15 );
	EXPECT_EQ( str.Find_NoCase( "aBCdABG" ) , -1 );

	SArray<SString> tokens;
	str.Split( tokens , "ABC" );
	EXPECT_EQ( tokens.GetSize() , 4 );

	str.Split( tokens , "ABCD" );
	EXPECT_EQ( tokens.GetSize() , 3 );
	EXPECT_STREQ( tokens[0].AsChar() , "BBC " );
	EXPECT_STREQ( tokens[1].AsChar() , "AB " );
	EXPECT_STREQ( tokens[2].AsChar() , "ABDE" );

	str = "D:\\Data\\Function\\";
	str.Split( tokens , "\\" );
	EXPECT_EQ( tokens.GetSize() , 3 );
	EXPECT_STREQ( tokens[0].AsChar() , "D:" );
	EXPECT_STREQ( tokens[1].AsChar() , "Data" );
	EXPECT_STREQ( tokens[2].AsChar() , "Function" );
}

TEST( SandBaseTest , IntVectorConstruct )
{
	SArray<int> vec_0;
	SArray<int> vec_1( { 1,2,3,4 } );
	SArray<int> vec_2 = { 0,3,4,5,6 };
	SArray<int> vec_3 = vec_2;

	EXPECT_EQ( vec_0.GetSize() , 0 );
	EXPECT_EQ( vec_0.GetCapacity() , 0 );

	EXPECT_EQ( vec_1.GetSize() , 4 );
	EXPECT_GE( vec_1.GetCapacity() , vec_1.GetSize() );
	EXPECT_EQ( vec_1[0] , 1 );
	EXPECT_EQ( vec_1[1] , 2 );
	EXPECT_EQ( vec_1[2] , 3 );
	EXPECT_EQ( vec_1[3] , 4 );

	EXPECT_EQ( vec_2.GetSize() , 5 );
	EXPECT_GE( vec_2.GetCapacity() , vec_2.GetSize() );
	EXPECT_EQ( vec_2[0] , 0 );
	EXPECT_EQ( vec_2[1] , 3 );
	EXPECT_EQ( vec_2[2] , 4 );
	EXPECT_EQ( vec_2[3] , 5 );
	EXPECT_EQ( vec_2[4] , 6 );

	EXPECT_EQ( vec_3.GetSize() , vec_2.GetSize() );
	EXPECT_EQ( vec_3.GetCapacity() , vec_2.GetCapacity() );
	EXPECT_EQ( vec_3[0] , vec_2[0] );
	EXPECT_EQ( vec_3[1] , vec_2[1] );
	EXPECT_EQ( vec_3[2] , vec_2[2] );
	EXPECT_EQ( vec_3[3] , vec_2[3] );
	EXPECT_EQ( vec_3[4] , vec_2[4] );
}

TEST( SandBaseTest , Swap )
{
	SArray<int> A = { 1,3,5 };
	SArray<int> B = { 88,35,12,6745,323 };

	int originSizeA = A.GetSize();
	int originSizeB = B.GetSize();

	int originCapacityA = A.GetCapacity();
	int originCapacityB = B.GetCapacity();

	A.Swap( B );

	EXPECT_EQ( A.GetSize() , originSizeB );
	EXPECT_EQ( A.GetCapacity() , originCapacityB );
	EXPECT_EQ( B.GetSize() , originSizeA );
	EXPECT_EQ( B.GetCapacity() , originCapacityA );

	EXPECT_EQ( A[0] , 88 );
	EXPECT_EQ( A[1] , 35 );
	EXPECT_EQ( A[2] , 12 );
	EXPECT_EQ( A[3] , 6745 );
	EXPECT_EQ( A[4] , 323 );

	EXPECT_EQ( B[0] , 1 );
	EXPECT_EQ( B[1] , 3 );
	EXPECT_EQ( B[2] , 5 );
}

#include "SandBase/Math/SMatrix3f.h"
#include "SandBase/Math/SVector3f.h"

TEST( SandBaseTest , Matrix3fMulVector3f )
{
	SMatrix3f mat( 1 , 2 , 3 ,
				   5 , 3 , 4 ,
				   7 , 4 , 5 );

	SVector3f v( 5 , 6 , 6 );

	const SVector3f& right_result = mat * v;
	const SVector3f& left_result  = v * mat;

	EXPECT_EQ( right_result[0] , 35 );
	EXPECT_EQ( right_result[1] , 67 );
	EXPECT_EQ( right_result[2] , 89 );

	EXPECT_EQ( left_result[0] , 77 );
	EXPECT_EQ( left_result[1] , 52 );
	EXPECT_EQ( left_result[2] , 69 );
}

#include "SandBase/Math/SMatrix4f.h"
#include "SandBase/Math/SVector4f.h"

TEST( SandBaseTest , Matrix4fMulVector4f )
{
	SMatrix4f mat( 1 , 4 , 2 , 2 ,
				   4 , 3 , 5 , 6 ,
				   8 , 1 , 3 , 2 ,
				   2 , 4 , 5 , 1 );

	SVector4f v( 1 , 2 , 3 , 4 );

	const auto& right = mat * v;
	const auto& left = v * mat;

	EXPECT_EQ( right[0] , 23 );
	EXPECT_EQ( right[1] , 49 );
	EXPECT_EQ( right[2] , 27 );
	EXPECT_EQ( right[3] , 29 );

	EXPECT_EQ( left[0] , 41 );
	EXPECT_EQ( left[1] , 29 );
	EXPECT_EQ( left[2] , 41 );
	EXPECT_EQ( left[3] , 24 );
}

#include "SandBase/Math/SQuaternion.h"

TEST( SandBaseTest , Quaternion )
{

}

#include "SandBase/Log/SLog.h"

TEST( SandBaseTest , SLog )
{
	SLog::Error( "I'm %s" , "error" );
	SLog::Warning( "I'm %s" , "warning" );
	SLog::Info( "I'm %s" , "info" );
}