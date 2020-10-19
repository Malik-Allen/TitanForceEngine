#ifndef VECTOR3_H
#define VECTOR3_H

#include "Constants.h"

#include <string>
#include <iostream>

namespace Kobe
{


	struct Vector3
	{

		float			x, y, z;

	private:

		// Utitily Load Function
		inline void Load( float X, float Y, float Z )
		{
			x = X; y = Y; z = Z;
		}

	public:

		// Utility Print Function
		inline void print()
		{
			printf( "( X = %1.8f, \tY = %1.8f, \tZ = %1.8f )\n", x, y, z );
		}


		// Default Constructor
		inline explicit Vector3( float s = float( 0.0 ) )
		{
			Load( s, s, s );
		}


		inline Vector3( float X, float Y, float Z )
		{
			Load( X, Y, Z );
		}

		// Copy Constructor
		inline Vector3( const Vector3& v )
		{
			Load( v.x, v.y, v.z );
		}

		// Assignment overload
		inline Vector3& operator=( const Vector3& v )
		{
			Load( v.x, v.y, v.z );
			return *this;
		}

		// Overload thats return conjugate vector
		inline const Vector3 operator - () const
		{
			return Vector3( -x, -y, -z );
		}

		// In order to use Vector3 as an array two overloaded operators are required
		// This overloaded operators allows Vector3 to be read as an array
		inline const float& operator[] ( int index ) const
		{
			return *(&x + index);	// Returns a dereferenced value of the address of the first component in this Vector3
		}

		// This overloaded operator allows Vector3 to be written as an array
		inline float& operator[] ( int index )
		{
			return *(&x + index);
		}

		// Addition overload, returning the sum of two Vector3, adding their components
		inline const Vector3 operator + ( const Vector3& v ) const
		{
			return Vector3( x + v.x, y + v.y, z + v.z );
		}

		// Subtraction overload, returns the difference between two Vector3, subtracting their components
		inline const Vector3 operator - ( const Vector3& v ) const
		{
			return Vector3( x - v.x, y - v.y, z - v.z );
		}

		// Addition and Assignment Overload
		inline Vector3& operator += ( const Vector3& v )
		{
			x += v.x;
			y += v.y;
			z += v.z;

			return *this;
		}

		// Subtraction and Assignment Overload
		inline Vector3& operator -= ( const Vector3& v )
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;

			return *this;
		}

		// Mulitplacation overload, multiplying this Vector3 by some scalar
		inline const Vector3 operator * ( const float s ) const
		{
			return Vector3( s * x, s * y, s * z );
		}

		// Multiplication and Assignment overload, between a Vector3 and Scalar
		inline Vector3& operator *= ( const float s )
		{
			x *= s;
			y *= s;
			z *= s;
			return *this;
		}

		// Division overload, dividing this Vector3 by some scalar
		inline const Vector3 operator / ( const float s ) const
		{

#ifdef _DEBUG

			if ( fabs( s ) < VERY_SMALL )
			{	// Checking to see if we are dividing by zero
				std::string errorMsg( "Error! Dividing by nearly zero!" );
				throw errorMsg;		// Throws an error message if we do
			}

#endif

			float r = 1.0f / s;
			return *this * r;
		}

		// Divison and Assignment overload, between a Vector3 and Scalar
		inline Vector3& operator /= ( const float s )
		{

#ifdef _DEBUG

			if ( fabs( s ) < VERY_SMALL )
			{	// Checking if dividing by zero
				std::string errorMsg( "Error! Dividing by nearly zero!" );
				throw errorMsg;		// Throws an error if you are
			}

#endif

			float r = 1.0f / s;
			return *this *= r;		// Multiplying by the fractional version of s
		}



		///
		/// Type conversion operators 
		///
		inline operator const float* () const
		{
			return static_cast<const float*>(&x);
		}

		inline operator float* ()
		{
			return static_cast<float*>(&x);
		}

		// Workaround, to allow for muliplication with a scalar first
		inline friend Vector3 operator * ( const float s, const Vector3& v )
		{
			return v * s;
		}


		// Returns the magnitude of this Vector3
		inline float magnitude() const
		{
			return sqrt( pow( x, 2.0f ) + pow( y, 2.0f ) + pow( z, 2.0f ) );
		}

		// Returns the unit vector of this Vector3
		Vector3 normalized() const
		{

			float a = magnitude();

#ifdef _DEBUG
			if ( fabs( a ) < VERY_SMALL )
			{	// Checking to see that the magnitude is not zero
				std::string errorMsg( "Error! Dividing nearly by zero!" );
				throw errorMsg;		// Throw an error if it is
			}
#endif
			return Vector3( x / a, y / a, z / a );


		}

		// Normalize this vector, changing its magnitude to one, but keeps its direction
		void Normalize()
		{

			float a = magnitude();

#ifdef _DEBUG
			if ( fabs( a ) < VERY_SMALL )
			{	// Checking to see that the magnitude is not zero
				std::string errorMsg( "Error! Dividing nearly by zero!" );
				throw errorMsg;		// Throw an error if it is
			}
#endif
			Load( x / a, y / a, z / a );

		}

		// Multiplies two Vector3 component-wise
		void Scale( const Vector3& v )
		{

			Load( x * v.x, y * v.y, z * v.z );

		}

	};

}




#endif // !VECTOR3_H

