#ifndef VECTOR4_H
#define VECTOR4_H

#include "Constants.h"
#include "Vector3.h"

namespace Kobe
{


	struct Vector4 : public Vector3
	{

		float			w;

	private:

		// Utitily Load Function
		inline void Load( float x_, float y_, float z_, float w_ )
		{
			x = x_; y = y_; z = z_; w = w_;
		}

	public:


		// Utility Print Function
		inline void print()
		{
			printf( "( X = %1.8f, \tY = %1.8f, \tZ = %1.8f, W = %1.8f )\n", x, y, z, w );
		}

		// Default Constructor
		inline explicit Vector4( float s = float( 0.0 ) )
		{
			x = s; y = s; z = s; w = s;
		}


		inline Vector4( float X, float Y, float Z, float W )
		{
			x = X; y = Y; z = Z; w = W;
		}

		// Copy Constructor
		inline Vector4( const Vector4& v )
		{
			Load( v.x, v.y, v.z, v.w );
		}

		// Constructor from Vector3, w = 1.0f
		inline Vector4( const Vector3& v )
		{
			Load( v.x, v.y, v.z, 1.0f );
		}

		// Assignment overload
		inline Vector4& operator=( const Vector4& v )
		{
			x = v.x; y = v.y; z = v.z; w = v.w;
			return *this;
		}

		// Overload thats return conjugate vector
		inline const Vector4 operator - () const
		{
			return Vector4( -x, -y, -z, -w );
		}


		/// In order to make Vector4 an array two overloaded operators are required
		/// This one allows Vector4 to be read as an array, const meaning it won't be changed
		inline const float& operator [] ( int index ) const
		{
			return *(&x + index);	// Returning a dereferenced address of the first component of Vector4 returns the lvalue at address
		}

		/// This overloaded operator allows Vector4 to written as ana array, hence it is not const
		inline float& operator [] ( int index )
		{
			return *(&x + index);
		}

		// Addition overload, returning the sum of two Vector4, adding their components
		inline const Vector4 operator + ( const Vector4& v ) const
		{
			return Vector4( x + v.x, y + v.y, z + v.z, w + v.w );
		}

		// Subtraction overload, returns the difference between two Vector4, subtracting their components
		inline const Vector4 operator - ( const Vector4& v ) const
		{
			return Vector4( x - v.x, y - v.y, z - v.z, w - v.w );
		}

		// Addition and Assignment Overload
		inline Vector4& operator += ( const Vector4& v )
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}

		// Subtraction and Assignment Overload
		inline Vector4& operator -= ( const Vector4& v )
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}

		// Mulitplacation overload, multiplying this Vector4 by some scalar
		inline const Vector4 operator * ( const float s ) const
		{
			return Vector4( x * s, y * s, z * s, w * s );
		}

		// Multiplication and Assignment overload, between a Vector4 and Scalar
		inline Vector4& operator *= ( const float s )
		{
			x *= s;
			y *= s;
			z *= s;
			w *= s;
			return *this;
		}

		// Division overload, dividing this Vector3 by some scalar
		inline const Vector4 operator / ( const float s ) const
		{
#ifdef _DEBUG
			if ( fabs( s ) < VERY_SMALL )
			{	// Checking to see if you are dividing by zero
				std::string errorMsg( "Error! Dividing by nearly zero!" );
				throw errorMsg;		// Throws an error is you are dividing by zero
			}
#endif
			float r = 1.0f / s;
			return *this * r;
		}

		// Divison and Assignment overload, between a Vector4 and Scalar
		inline Vector4& operator /= ( const float s )
		{
#ifdef _DEBUG
			if ( fabs( s ) < VERY_SMALL )
			{
				std::string errorMsg( "Error! Dividing by nearly zero!" );
				throw errorMsg;
			}
#endif
			float r = 1.0f / s;
			return *this *= r;
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
		inline friend Vector4 operator * ( const float s, const Vector4& v )
		{
			return v * s;
		}

		// Returns the magnitude of this Vector3
		inline float magnitude() const
		{
			return sqrt( pow( x, 2.0f ) + pow( y, 2.0f ) + pow( z, 2.0f ) + pow( w, 2.0f ) );
		}

		// Returns the unit vector of this Vector4
		Vector4 normalized() const
		{
			float a = sqrt( pow( x, 2.0f ) + pow( y, 2.0f ) + pow( z, 2.0f ) + pow( w, 2.0f ) );
#ifdef _DEBUG
			if ( fabs( a ) < VERY_SMALL )
			{
				std::string errorMsg( "Error! Dividing by nearly zero!" );
				throw errorMsg;
			}
#endif
			return Vector4( x / a, y / a, z / a, w / a );
		}

		// Normalize this vector, changing its magnitude to one, but keeps its direction
		void Normalize()
		{
			float a = sqrt( pow( x, 2.0f ) + pow( y, 2.0f ) + pow( z, 2.0f ) + pow( w, 2.0f ) );
#if _DEBUG
			if ( fabs( a ) < VERY_SMALL )
			{
				std::string errorMsg( "Error! Dividing nearly by zero!" );
				throw errorMsg;
			}
#endif

			Load( x /= a, y /= a, z /= a, w /= a );

		}

		// Multiplies two Vector4 component-wise
		void Scale( const Vector4& v )
		{
			Load( x * v.x, y * v.y, z * v.z, w * v.w );
		}

	};

}



#endif // !VECTOR4_H

