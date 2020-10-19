#ifndef MATRIX3_H
#define MATRIX3_H

#include "Matrix4.h"

namespace Kobe
{

	// Class representing a column major 3x3 matrix
	class Matrix3
	{

		float m[9];

	public:
		// Default Conststructor
		inline explicit Matrix3( float d = float( 0.0 ) )
		{
			if ( d == 0.0f )	// When the passed value is zero, lets create an identity matrix
				LoadIdentity();
			else
			{			// Otherwise populate the matrix with the value of d
				m[0] = d;	m[1] = d;	m[2] = d;
				m[3] = d;	m[4] = d;	m[5] = d;
				m[6] = d;	m[7] = d;	m[8] = d;
			}
		}

		// Utility to populate a Matrix3
		inline Matrix3( float xx, float xy, float xz,
						float yx, float yy, float yz,
						float zx, float zy, float zz )
		{
			m[0] = xx;	m[1] = xy; m[2] = xz;
			m[3] = yx; m[4] = yy; m[5] = yz;
			m[6] = zx; m[7] = zy; m[8] = zz;
		}

		// Utility to populate a Matrix3
		inline void Load( float xx, float xy, float xz,
						  float yx, float yy, float yz,
						  float zx, float zy, float zz )
		{
			m[0] = xx;	m[1] = xy;	m[2] = xz;
			m[3] = yx;	m[4] = yy;	m[5] = yz;
			m[6] = zx;	m[7] = zy;	m[8] = zz;
		}

		// In order to use Matrix3 as an array I requires two overloaded operators
		// One to read Matrix 3 as if it were an array 
		inline const float operator [] ( int index ) const
		{
			return *(m + index);
		}

		// One to write Matrix3 as an array
		inline float& operator [] ( int index )
		{
			return *(m + index);
		}

		// Overloaded assignment operator
		inline Matrix3& operator=( const Matrix3& m_ )
		{
			this->m[0] = m_[0];	this->m[1] = m_[1];	this->m[2] = m_[2];
			this->m[3] = m_[3];	this->m[4] = m_[4];	this->m[5] = m_[5];
			this->m[6] = m_[6];	this->m[7] = m_[7];	this->m[8] = m_[8];
			return *this;
		}

		// Load an identity matrix
		inline void LoadIdentity()
		{
			this->m[0] = 1.0f; this->m[1] = 0.0f; this->m[2] = 0.0f;
			this->m[3] = 0.0f; this->m[4] = 1.0f; this->m[5] = 0.0f;
			this->m[6] = 0.0f; this->m[7] = 0.0f; this->m[8] = 1.0f;
		}

		// Populate an Diagonal Matrix with a value
		inline void LoadIdentity( float d )
		{
			if ( d == 0.0f )
				d = 1.0f;

			this->m[0] = d;	this->m[1] = 0.0f;	this->m[2] = 0.0f;
			this->m[3] = 0.0f;	this->m[4] = d;	this->m[5] = 0.0f;
			this->m[6] = 0.0f;	this->m[7] = 0.0f;	this->m[8] = d;
		}

		// Overloaded multiplication operator between two Matrix3 
		inline const Matrix3 operator * ( const Matrix3& n ) const
		{
			Matrix3 result;
			for ( int i = 0; i < 3; i++ )
			{
				for ( int j = 0; j < 3; j++ )
				{
					result[i * 3 + j] = (m[0 * 3 + j] * n[i * 3 + 0]) + (m[1 * 3 + j] * n[i * 3 + 1]) + (m[2 * 3 + j] * n[i * 3 + 2]);
				}
			}
			return result;
		}

		// Overloaded multiplication of two Matrix3 and asignment onto self
		inline Matrix3& operator *= ( const Matrix3& n )
		{
			*this = *this * n;
			return *this;
		}

		// Overloaded multiplication operator between a Vector3 and Matrix3
		inline const Vector3 operator * ( const Vector3& v )
		{
			Vector3 result;
			result.x = m[0] * v.x + m[1] * v.y + m[2] * v.z;
			result.y = m[3] * v.x + m[4] * v.y + m[5] * v.z;
			result.z = m[6] * v.x + m[7] * v.y + m[8] * v.z;
			return result;
		}


		// Extracts the inner 3x3 from a 4x4 matrix
		// using the assignment operator
		inline Matrix3& operator = ( const Matrix4& m_ )
		{
			this->m[0] = m_[0]; this->m[1] = m_[1]; this->m[2] = m_[2];
			this->m[3] = m_[4]; this->m[4] = m_[5]; this->m[5] = m_[6];
			this->m[6] = m_[8]; this->m[7] = m_[9]; this->m[8] = m_[10];
			return *this;
		}

		// Extracts the inner 3x3 from a 4x4 matrix
		// using the constructor
		inline Matrix3( const Matrix4& m_ )
		{
			this->m[0] = m_[0]; this->m[1] = m_[1]; this->m[2] = m_[2];
			this->m[3] = m_[4]; this->m[4] = m_[5]; this->m[5] = m_[6];
			this->m[6] = m_[8]; this->m[7] = m_[9]; this->m[8] = m_[10];
		}

		// Print all the compoents of the assignment
		inline void print()
		{
			printf( "%1.8f\t%1.8f\t%1.8f\n%1.8f\t%1.8f\t%1.8f\n%1.8f\t%1.8f\t%1.8f\n\n",
					m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8] );
		}


	};


}


#endif // !MATRIX3_H

