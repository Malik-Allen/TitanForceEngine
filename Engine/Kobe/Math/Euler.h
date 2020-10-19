#ifndef EULER_H
#define EULER_H

#include "Vector3.h"

namespace Kobe
{

	// Structure representing the an Euler Angle
	struct Euler : public Vector3
	{
		// The angles in the Euler Struct should never exceed 360.0
		// Representing a sequence of rotations
		// pitch, yaw, roll;


		// Default constructor for the Euler Struct
		inline explicit Euler( float a = float( 0.0 ) )
		{
			x = a; y = a; z = a;
		}

		// A constructor populating the components of a Euler
		inline Euler( float pitch_, float yaw_, float roll_ )
		{
			Load( pitch_, yaw_, roll_ );
		}

		// Utility used to populate the components of a Euler
		inline void Load( float pitch_, float yaw_, float roll_ )
		{
			x = pitch_; y = yaw_; z = roll_;
		}

		// A copy constructor
		inline Euler( const Euler& e )
		{
			x = e.x; y = e.y; z = e.z;
		}

		// An assignment operator
		inline Euler& operator= ( const Euler& e )
		{
			x = e.x; y = e.y; z = e.z;
			return *this;
		}

	};

}



#endif // !EULER_H

