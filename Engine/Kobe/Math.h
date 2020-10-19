#ifndef MATH_H
#define MATH_H

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix3.h"
#include "Math/Matrix4.h"
#include "Math/Euler.h"
#include "Math/Quaternion.h"

namespace Kobe {	

	enum class AngleUnitOfMeasurement
	{
		Degress = 0,
		Radians
	};

	
	// Static Math Class, Holding All Mathematical Functionality
	class Math {

		Math() = delete;
		~Math() = delete;
		Math (const Math&) = delete;
		Math& operator=(const Math&) = delete;
		Math (Math&&) = delete;
		Math& operator=(const Math&&) = delete;

	public:

		///// ***** VECTOR2 ***** \\\\\

		// Calculates the dot product between two Vector2
		static float Dot(const Vector2& v1, const Vector2& v2);

		// Calculates the length between two Vector2
		static float Distance(const Vector2& v1, const Vector2& v2);

		// Calculates the angle between two Vector2 in radians
		static float Angle(const Vector2& v1, const Vector2& v2, const AngleUnitOfMeasurement& unit = AngleUnitOfMeasurement::Radians);

		// Linear Interpolation between starting vector2 and end vector2 at rate t
		// When t = 0 returns startPosition. 
		// When t = 1 returns end position. 
		// When t = 0.5 returns midway between start and end positions.
		static Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t);

		// Reflects a Vector2 on a Vector2 defined by its normal
		static Vector2 Reflect(const Vector2& v, const Vector2& n);

		// Returns vector projection of vector u onto vector v
		Vector2 Project( const Vector2& u, const Vector2& v );




		///// ***** VECTOR3 ***** \\\\\

		// Calculates the normal Vector3
		static Vector3 normalize(const Vector3& v);

		// Calculates the dot product of two Vector3
		static float Dot(const Vector3& v1, const Vector3& v2);

		// Calculates the length between two Vector3
		static float Distance(const Vector3& v1, const Vector3& v2);

		// Calculates the angle between two Vector3
		static float Angle(const Vector3& v1, const Vector3& v2, const AngleUnitOfMeasurement& unit = AngleUnitOfMeasurement::Radians);

		// Linear Interpolation between starting vector3 and end vector3 at rate t
		// When t = 0 returns startPosition
		// When t = 1 returns end position
		// When t = 0.5 returns midway between start and end positions
		static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

		// Reflects a Vector3 on a Vector3 defined by its normal
		static Vector3 Reflect(const Vector3& v, const Vector3& n);

		// Calculates the cross product between two Vector3
		static Vector3 Cross(const Vector3& a, const Vector3& b);

		// Returns vector projection of vector u onto vector v
		static Vector3 Project(const Vector3& u, const Vector3& v);


		// Calculates the perpindicular normal to the passed in Vector3's normal
		// The result can be infinite, so pass through a the x, and y values you would like to work with
		// The passed Vector3 must have a non-zero z-component
		static Vector3 OrthoNormalize(const Vector3& v, float x_, float y_);

		// Calculates the Spherical Linear Interpolation between the passed starting point and end position at the rate of t
		// If the angle between the two Vector3 is less than 10 degrees than we use linear interpolation
		// When t = 0 returns startPosition 
		// When t = 1 returns end position 
		// When t = 0.5 returns midway between start and end positions
		static Vector3 SLerp(const Vector3& v1, const Vector3& v2, float t);




		///// ***** VECTOR4 ***** \\\\\

		// Calculates the dot product between two Vector4
		static float Dot(const Vector4& v1, const Vector4& v2);

		// Calculates the length between two Vector4
		static float Distance(const Vector4& v1, const Vector4& v2);

		// Linear Interpolation between starting vector4 and end vector4 at rate t
		// When t = 0 returns startPosition
		// When t = 1 returns end position
		// When t = 0.5 returns midway between start and end positions
		static Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t);

		// Calculates Projection of one Vector4 onto another Vector4
		static Vector4 Project(const Vector4& u, const Vector4& v);



		///// ***** MATRIX4 ***** \\\\\
		/// Matrix Linear Transformations are preformed on 4x4 Matrices, so always convert a 3x3 Matrix to a 4x4 before transformations
		/// This is account for Translation which is a affine transformation which requires a 4x4 Matrix


		// Rotates this Matrix4 about any of the provided axes, and the given the degrees of rotation
		static Matrix4 Rotate( const float degrees, const float x, const float y, const float z );

		// Rotates this Matrix4 about any of the provided axes as a Vector3, and the given the degrees of rotation
		static Matrix4 Rotate( const float degrees, const Vector3& axis );

		// Preforms uniform scaling, provided the scale factor on each axis
		static Matrix4 Scale( const float x, const float y, const float z );

		// Preforms uniform scaling, provided the scale factor on each axis as a Vector3
		static Matrix4 Scale( const Vector3& scale );

		// Translates this Matrix4 across the given direction components
		static Matrix4 Translate( const float x_, const float y_, const float z_ );

		// Translates this Matrix4 across the given Vector3 direction components
		static Matrix4 Translate( const Vector3& translate_ );

		// Geometric operation of Perspective projection
		static Matrix4 Perspective( const float fovy_, const float aspect_, const float zNear_, const float zFar_ );

		// This creates a transform from Normalized Device Coordinates (NDC) to screen coordinates.
		static Matrix4 ViewportNDC( const int width_, const int height_ );

		// This Orthographic function is a dimension reducing operatorion
		static Matrix4 Orthographic( const float xMin_, const float xMax_,
									 const float yMin_, const float yMax_,
									 const float zMin_, const float zMax_ );


		static Matrix4 UnOrtho( const Matrix4& ortho );

		// Look At functions creating the eye, look at position and up position of a camera
		static Matrix4 LookAt( const float eyeX_, const float eyeY_, const float eyeZ_,
							   const float atX_, const float atY_, const float atZ_,
							   const float upX_, const float upY_, const float upZ_ );

		// Look At functions creating the eye, look at position and up position of a camera
		static Matrix4 LookAt( const Vector3& eye, const Vector3& at, const Vector3& up );

		// Switchs the columns with the rows in the Matrix4
		static Matrix4 Transpose( const Matrix4& m );

		// Returns the inverse of a Matrix4
		static Matrix4 Inverse( const Matrix4& m );



		///// ***** QUATERNION ***** \\\\\

		// Calculates the normalized conjugate of a Quaternion
		static Quaternion Inverse( const Quaternion& q );

		// Returns the conjugate of the passed Quaternion
		static Quaternion Conjugate( const Quaternion& q );

		// Calculates the unit quaternion
		static Quaternion Normalize( const Quaternion& q );

		// Calculates the dot product between two quaternions
		static float Dot( const Quaternion& q1, const Quaternion& q2 );

		// **WIP::Calculates the angle between two Quaternions
		static float Angle( const Quaternion& q1, const Quaternion& q2 );

		// Calculates the rotation of a Vector3 using the passed Quaternion
		static Vector3 Rotate( const Quaternion& q, const Vector3& v );

		// **WIP::Linear Interpolation between starting Quaternion and end Quaternion at rate t
		// When t = 0 returns startPosition
		// When t = 1 returns end position
		// When t = 0.5 returns midway between start and end positions
		static Quaternion Lerp( const Quaternion& q1, const Quaternion& q2, float t );

		// **WIP:: Calculates the Spherical Linear Interpolation between the passed starting point and end position at the rate of t
		// If the angle between the two Vector3 is less than 10 degrees than we use linear interpolation
		// When t = 0 returns startPosition 
		// When t = 1 returns end position 
		// When t = 0.5 returns midway between start and end positions
		static Quaternion SLerp( const Quaternion& q1, const Quaternion& q2, float t );




		///// ***** Conersions ***** \\\\\

		// Converting the representation of a Euler to a Matrix4
		static Matrix4 EulerToMatrix( const Euler& e );

		// Converting the representation of a Matrix4 to an Euler
		static Euler MatrixToEuler( const Matrix4& m );

		// Converting a Quaternion into a 4x4 Matrix
		static Matrix4 QuaternionToMatrix( const Quaternion& q );

		// Converting a Matrix into a Quaternion
		static Quaternion MatrixToQuaternion( const Matrix4& m );

		// Converting an Euler(Y, Z, X) into a Quaternion
		static Quaternion EulerToQueaternion( const Euler& e );

		// Converting a Quaternion into an Euler(Y, Z, X)
		static Euler QuaternionToEuler( const Quaternion& q );



	};

}


#endif

// Note 1:
/// When working with arrays be careful... an array can be defined to have a number of blocks but you can override that and pop the stack either way



// Note 3:
/// Possible that OrthoNormalize may not work the way I intended because of the possibility of z-component being zero
/// OrthoNormalize function must take a non-zero z-component from the passed the Vector3 in its arguement
/// I use the z-component to solve for the orthogonal's zero component

// Note 4:
/// SLerp should be multiplied by some scalar when in use to change the scale/ radius of the spherical linear interpolation

// Note 5:
/// Matrix Linear Transformations are preformed on 4x4 Matrices, so always convert a 3x3 Matrix to a 4x4 before trnasformations
/// This is account for Translation which is a affine transformation which requires a 4x4 Matrix

// Note 6:
/// Coversions of Eulers and Quatrnions follow the yaw, roll, pitch order
