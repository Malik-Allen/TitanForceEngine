#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "Debug.h"
#include "Vector.h"

class Algebra{

public:

	// Returns the discrimnant of the paramters passed
	inline static float CalculateDiscriminant(float a, float b, float c) {
		return (b * b) - (4.0f * a * c);
	}

	// Return the value the quadrtaic solution of the provided coefficients
	// Requires a boolean to determine whether or not to return the smaller(true) and larger(false) solution to the quadratic
	inline static float CalculateQuadratic(float a, float b, float c, bool returnSmallest) {
		float discriminant = CalculateDiscriminant(a, b, c);

		if (discriminant < 0.0f) {															// Check if the discriminant is less than zerp
			Debug::Error("Discriminant is less than zero!", __FILE__, __LINE__);
			return VERY_SMALL;																// Return a zero and an error if so, because you cannot take the square root of a negative number
		}
		else if (CalculateDiscriminant(a, b, c) == 0.0f) {									// If the discrimnant is equal to zero there is only one solution

			return ((-1.0f * b) + sqrt(discriminant)) / (2.0f * a);
			
		}
		else if (discriminant > 0.0f) {														// If the discriminant is greater than zero than there are two solutions

			float t1 = ((-1.0f * b) + sqrt(discriminant)) / (2.0f * a);
			float t2 = ((-1.0f * b) - sqrt(discriminant)) / (2.0f * a);

			if (returnSmallest) {															// If returnSmallest is true return the smaller of the two solutions
				if (t1 > t2)
					return t2;
				else 
					return t1;
			}
			else if (!returnSmallest) {														// If returnSmallest is false return the larger of the two solutions
				if (t1 < t2)
					return t2;
				else
					return t1;
			}
		}
	}


};
#endif