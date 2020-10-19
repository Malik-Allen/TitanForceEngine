#ifndef QUATERNION_H
#define QUATERNION_H

#include "Constants.h"
#include "Vector4.h"
#include "Euler.h"

namespace Kobe
{

	// Structure representing a Quaternion 
	struct Quaternion : public Vector4
	{

		// Default constructor for the Quaternion class
		inline explicit Quaternion( float s = float( 0.0 ) )
		{
			x = s; y = s; z = s;
			(s == 0.0f) ? w = 1.0f : w = s;
		}

		// A constructor that populates the components of the Quaternion
		inline Quaternion( float x_, float y_, float z_, float w_ )
		{
			Load( x_, y_, z_, w_ );
		}

		// Utility to populate the Quaternion components
		inline void Load( float x_, float y_, float z_, float w_ )
		{
			x = x_; y = y_; z = z_; w = w_;
		}

		// A constructor taking the axis and degrees of rotation
		inline Quaternion( const Vector3& axis, float degrees )
		{
			degrees *= DEGREES_TO_RADIANS;
			w = cos( degrees / 2.0f );
			x = axis.x * sin( degrees / 2.0f );
			y = axis.y * sin( degrees / 2.0f );
			z = axis.z * sin( degrees / 2.0f );
		}

		// A copy constructor
		inline Quaternion( const Quaternion& q )
		{
			x = q.x; y = q.y; z = q.z; w = q.w;
		}

		// An assignment operator
		inline Quaternion& operator=( const Quaternion& q )
		{
			x = q.x; y = q.y; z = q.z; w = q.w;
			return *this;
		}

		// **WIP:: Preforms the addition between two quaternions
		inline const Quaternion operator + ( const Quaternion& q ) const
		{
			return Quaternion( x + q.x, y + q.y, z + q.z, w + q.w );
		}

		// Preforms the multiplication between two quaternions
		inline const Quaternion operator * ( const Quaternion& q ) const
		{
			Vector3 v1( x, y, z );
			Vector3 v2( q.x, q.y, q.z );

			float w_ = (w * q.w) - (x * q.x + y * q.y + z * q.z);
			Vector3 v = (w * v2) + (q.w * v1) + ( Vector3( v1.y * v2.z - v1.z * v2.y, -(v1.x * v2.z - v1.z * v2.x), v1.x * v2.y - v1.y * v2.x ) );

			return Quaternion( v.x, v.y, v.z, w_ );
		}

		// Preforms the multiplication between a scalar and a quaternion
		inline const Quaternion operator * ( const float t ) const
		{
			return Quaternion( x * t, y * t, z * t, w * t );
		}

		/// Current work around to allow scalar quaternion multiplication with the scalar first 
		inline friend Quaternion operator * ( const float t, const Quaternion& q )
		{
			return q * t;
		}

		// Preforms the multiplication between a scalar and a quaternion and assignment onto it self
		inline Quaternion& operator *= ( const float t )
		{
			x *= t;
			y *= t;
			z *= t;
			w *= t;
			return *this;
		}

		// Returns the conjugate of this Quaternion, therfore reversing the Vector component of the Quaternion
		inline Quaternion conjugated() const
		{
			return Quaternion( -x, -y, -z, w );
		}

		// Returns the unit Quaternion of this Quaternion
		Quaternion normalized() const
		{
			float mag = sqrt( pow( x, 2.0f ) + pow( y, 2.0f ) + pow( z, 2.0f ) + pow( w, 2.0f ) );
#ifdef _DEBUG
			if ( fabs( mag ) < VERY_SMALL )
			{
				std::string errorMsg( "Error! Dividing by a magnitude of nearly zero!" );
				throw errorMsg;
			}
#endif
			

			return Quaternion( x / mag, y / mag, z / mag, w / mag);
		}

		// Normalizes this Quaternion
		void Normalize()
		{
			float mag = sqrt( pow( x, 2.0f ) + pow( y, 2.0f ) + pow( z, 2.0f ) + pow( w, 2.0f ) );
#ifdef _DEBUG
			if ( fabs( mag ) < VERY_SMALL )
			{
				std::string errorMsg( "Error! Dividing by a magnitude of nearly zero!" );
				throw errorMsg;
			}
#endif
			x /= mag;
			y /= mag;
			z /= mag;
			w /= mag;

		}

		// Returns the representation of this Quaternion as Euler Angles
		Euler euler() const
		{
			float sqw = w * w;
			float sqx = x * x;
			float sqy = y * y;
			float sqz = z * z;

			float sum = sqx + sqy + sqz + sqw;	// Calculating the summation of all 4 components
			float testCase = (x * y) + (z * w);

			float yaw, roll, pitch;

			if ( testCase > 0.499f * sum )
			{
				yaw = 2.0f * atan2( x, w );
				roll = PI / 2.0f;
				pitch = 0.0f;
			}
			else if ( testCase < -0.499f * sum )
			{
				yaw = -2.0f * atan2( x, w );
				roll = -PI / 2.0f;
				pitch = 0.0f;
			}
			else
			{
				yaw = atan2( (2.0f * y * w) - (2.0f * x * z), sqx - sqy - sqz + sqw );
				roll = asin( 2.0f * testCase / sum );
				pitch = atan2( (2.0f * x * w) - (2.0f * y * z), -sqx + sqy - sqz + sqw );
			}

			return Euler( pitch * RADIANS_TO_DEGREES, yaw * RADIANS_TO_DEGREES, roll * RADIANS_TO_DEGREES );
		}

		inline void print()
		{
			printf( "%1.8f\t%1.8f\t%1.8f\t%1.8f\n", x, y, z, w );
		}

	};

}



#endif // !QUATERNION_H
