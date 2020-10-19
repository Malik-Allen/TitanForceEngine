#ifndef VECTOR2_H
#define VECTOR2_H

#include "Constants.h"

#include <iostream>
#include <string>


namespace Kobe {

	struct Vector2 {

		
		float		x, y;

	private:

		// Utitily Load Function
		inline void Load(float X, float Y) 
		{
			x = X; y = Y;
		}

	public:

		// Utility Print Function
		inline void print() 
		{
			printf("( X = %1.8f, \tY = %1.8f )\n", x, y);
		}

		// Default Constructor
		inline explicit Vector2(float s = float(0.0)) 
		{ 
			Load(s, s);
		}

		inline Vector2(float X, float Y) 
		{
			Load(X, Y);
		}

		// Copy Constructor
		inline Vector2(const Vector2& v) 
		{
			Load(v.x, v.y);
		}

		// Assignment overload
		inline Vector2& operator=(const Vector2& v) 
		{
			Load(v.x, v.y);
			return *this;
		}

		// Overload thats return conjugate vector
		inline const Vector2 operator - () const 
		{
			return Vector2(-x, -y);
		}

		// In order to give Vector2 array functionality it requires 2 overloaded operators
		// This operator is used to read Vector2 as if it were an array
		inline const float operator [] (int index) const 
		{
			return *(&x + index);	// Returns a dereferenced address of the first component of this vector plus the index number
		}

		// This operator is used to write to Vector2 as if it were an array
		inline float& operator [] (int index) 
		{
			return *(&x + index);
		}

		// Addition overload, returning the sum of two Vector2, adding their components
		inline const Vector2 operator + (const Vector2& v) const 
		{
			return Vector2(x + v.x, y + v.y);
		}

		// Subtraction overload, returns the difference between two Vector2, subtracting their components
		inline const Vector2 operator - (const Vector2& v) const 
		{
			return Vector2(x - v.x, y - v.y);
		}

		// Addition and Assignment Overload
		inline Vector2& operator += (const Vector2& v) 
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		// Subtraction and Assignment Overload
		inline Vector2& operator -= (const Vector2& v) 
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		// Mulitplacation overload, multiplying this Vector2 by some scalar
		inline const Vector2 operator * (const float s) const 
		{
			return Vector2(s * x, s * y);
		}

		// Multiplication and Assignment overload, between a Vector2 and Scalar
		inline Vector2& operator *= (const float s) 
		{
			x *= s;
			y *= s;
			return *this;
		}

		// Division overload, dividing this Vector3 by some scalar
		inline const Vector2 operator / (const float s) const 
		{

#ifdef _DEBUG

			if (fabs(s) < VERY_SMALL) {	// Check to see if we are dividing to nearly zero
				std::string errorMsg("Error! Dividing by nearly zero!");
				throw errorMsg;
			}

#endif
			float r = 1.0f / s;
			return *this * r;

		}

		// Divison and Assignment overload, between a Vector3 and Scalar
		inline Vector2& operator /= (const float s) 
		{

#ifdef _DEBUG

			if (fabs(s) < VERY_SMALL) {
				std::string errorMsg("Error! Dividing by nearly zero!");
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
		inline friend Vector2 operator * (const float s, const Vector2& v) 
		{
			return v * s;
		}

		// Returns the magnitude of this Vector2
		inline float magnitude() const
		{
			return sqrt(pow(x, 2.0f) + pow(y, 2.0f));
		}

		// Returns the unit vector of this Vector2
		Vector2 normalized() const
		{
			float a = sqrt(pow(x, 2.0f) + pow(y, 2.0f));
#ifdef _DEBUG
			if (fabs(a) < VERY_SMALL)
			{	// Checking to see that the magnitude is not zero
				std::string errorMsg("Error! Dividing nearly by zero!");
				throw errorMsg;		// Throw an error if it is
			}
#endif
			return Vector2(x / a, y / a);
		}

		// Normalize this vector, changing its magnitude to one, but keeps its direction
		void Normalize()
		{
			float a = sqrt(pow(x, 2.0f) + pow(y, 2.0f));
#ifdef _DEBUG
			if (fabs(a) < VERY_SMALL)
			{	// Checking to see that the magnitude is not zero
				std::string errorMsg("Error! Dividing by nearly zero!");
				throw errorMsg;
			}
#endif

			Load(x /= a, y /= a);
		}

		// Multiplies two Vector2 component-wise
		void Scale(const Vector2& v)
		{
			Load(x * v.x, y * v.y);
		}

	};

}



#endif // !VECTOR2_H

