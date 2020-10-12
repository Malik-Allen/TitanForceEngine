#ifndef MATH_H
#define MATH_H

#include <iostream>
#include <string>	// For debug handling

namespace MATH {	// Wrapping everything in the identify the proper Vector being used

#ifndef VERY_SMALL
#define VERY_SMALL 1.0e-7f
#endif

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifndef DEGREES_TO_RADIANS
#define DEGREES_TO_RADIANS (PI / 180.0f)
#endif

#ifndef RADIANS_TO_DEGREES
#define RADIANS_TO_DEGREES (180.0f / PI)
#endif

	// Structure representing a 2D vector
	struct Vector2 {
		float x, y;

		// Normally these constructors should be deleted if they are not being used correctly, because it cause implicit things
		// Vector2 (const Vector2&) = delete;
		// Vector2& operator=(const Vector2&) = delete;
		// Vector2 (Vector2&&) = delete;
		// Vector2& operator=(const Vector2&&) = delete;
		
		/// Utility used to populate Vector2
		inline void Load(float x_, float y_) {
			x = x_; y = y_;
		}

		/// A default constructor for the Vector2 setting 
		inline explicit Vector2(float s = float(0.0)) { // Parameter 's' can be overridden by a passed float or double
			Load(s, s);
		}

		/// A constructor that populates the Vector2
		inline Vector2(float x_, float y_) {
			Load(x_, y_);
		}

		/// A copy constructor
		inline Vector2(const Vector2& v) {
			Load(v.x, v.y);
		}

		/// 'Assignment Operator' Overloaded equal operator Vector2
		inline Vector2& operator=(const Vector2& v) {
			Load(v.x, v.y);
			return *this;
		}

		/// Returns a negative of the Vector2
		inline const Vector2 operator - () const {
			return Vector2(-x, -y);
		}

		/// In order to give Vector2 array functionality it requires 2 overloaded operators

		/// This operator is used to read Vector2 as if it were an array
		inline const float operator [] (int index) const {
			return *(&x + index);	// Returns a dereferenced address of the first component of this vector plus the index number
		}

		/// This operator is used to write to Vector2 as if it were an array
		inline float& operator [] (int index) {
			return *(&x + index);	
		}

		/// Overloaded addition operator Vector2
		inline const Vector2 operator + (const Vector2& v) const {
			return Vector2(x + v.x, y + v.y);
		}

		/// Overloaded subtraction operator Vector2
		inline const Vector2 operator - (const Vector2& v) const {
			return Vector2(x - v.x, y - v.y);
		}

		/// Overloaded addition onto ones self operator
		inline Vector2& operator += (const Vector2& v) {
			x += v.x;
			y += v.y;
			return *this;
		}

		/// Overloaded subtraction onto ones self operator
		inline Vector2& operator -= (const Vector2& v) {
			x -= v.x;
			y -= v.y;
			return *this;
		}

		/// Overloaded scalar multiplication of Vector2
		inline const Vector2 operator * (const float s) const {
			return Vector2(s * x, s * y);
		}

		/// Overloaded scalar multiplication of a Vector2 assign it to itself
		inline Vector2& operator *= (const float s) {
			x *= s;
			y *= s;
			return *this;
		}

		/// Overloaded division operator of a Vector2 by a scalar
		inline const Vector2 operator / (const float s) const {
#ifdef _DEBUG
			if (fabs(s) < VERY_SMALL) {	// Check to see if we are dividing to nearly zero
				std::string errorMsg("Error! Dividing by nearly zero!");
				throw errorMsg;
			}
#endif
			float r = 1.0f / s;	
			return *this * r;

		}

		/// Overloaded division of Vector2 by a scalar and assignment onto itself
		inline Vector2& operator /= (const float s) {
#ifdef _DEBUG
			if (fabs(s) < VERY_SMALL) {
				std::string errorMsg("Error! Dividing by nearly zero!");
				throw errorMsg;
			}
#endif
			float r = 1.0f / s;
			return *this *= r;
		}

		/// This friend overloaded operator allows the multiplication between a Scalar and Vector2 where a scalar is first
		inline friend Vector2 operator * (const float s, const Vector2& v) {
			return v * s;
		}

		/// Utility to print the Vector2 components
		inline void print() {
			printf("%1.8f\t%1.8f\n", x, y);
		}

		///
		/// Type conversion operators 
		///
		inline operator const float* () const {
			return static_cast<const float*>(&x);
		}

		inline operator float* () {
			return static_cast<float*>(&x);
		}

		// Returns the Magnitude(length) of this Vector2
		inline float magnitude() {
			return sqrt(pow(x, 2.0f) + pow(y, 2.0f));
		}

		// Returns the unit vector of this Vector2
		Vector2 normalized();

		// Makes this Vector2 have a magnitude of 1
		void Normalize();

		// Multiplies two Vector2 component-wise
		void Scale(const Vector2& v);

	};

	// Strcutre representing a 3D vector
	struct Vector3 {
		float x, y, z;

		// A set of constructors providing implicit constructions that won't be needed, so we delete them
		//Vector3(const Vector3&) = delete;
		//Vector3& operator=(const Vector3&) = delete;
		//Vector3(Vector3&&) = delete;
		//Vector3& operator=(Vector3&&) = delete;

		/// Utility used to populate Vector3
		inline void Load(float x_, float y_, float z_) {
			x = x_; y = y_; z = z_;
		}

		/// A 'safe' default contructor for the Vector3 
		inline explicit Vector3(float s = float(0.0)) {	// Parameter 's' can be overridden by any passed float or double
			Load(s, s, s);
		}

		/// A constructor that populates Vector3
		inline Vector3(float x_, float y_, float z_) {
			Load(x_, y_, z_);
		}

		/// A copy constructor
		inline Vector3(const Vector3& v) {
			Load(v.x, v.y, v.z);
		}

		/// 'Assignment Operator' Overloaded equal operator Vector3
		inline Vector3& operator=(const Vector3& v) {
			Load(v.x, v.y, v.z);
			return *this;
		}

		/// Returns the negative of this Vector3
		inline const Vector3 operator - () const {
			return Vector3(-x, -y, -z);
		}

		/// In order to use Vector3 as an array two overloaded operators are required
		/// This overloaded operators allows Vector3 to be read as an array
		inline const float& operator[] (int index) const {
			return *(&x + index);	// Returns a dereferenced value of the address of the first component in this Vector3
		}

		/// This overloaded operator allows Vector3 to be written as an array
		inline float& operator[] (int index) {
			return *(&x + index);
		}

		/// Overloaded addition operator Vector3
		inline const Vector3 operator + (const Vector3& v) const {
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		/// Overloaded subtraction operator Vector3
		inline const Vector3 operator - (const Vector3& v) const {
			return Vector3(x - v.x, y - v.y, z - v.z);
			
		}

		/// Overloaded addition of a Vector3 onto one's self
		inline Vector3& operator += (const Vector3& v) {
			x += v.x;
			y += v.y;
			z += v.z;

			return *this;
		}

		/// Overloaded subtraction of a Vector3 onto one's self
		inline Vector3& operator -= (const Vector3& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;

			return *this;
		}

		/// Overloaded scalar multiplication Vector3 operator
		inline const Vector3 operator * (const float s) const {
			return Vector3(s * x, s * y, s * z);
		}

		/// Overloaded scalar multiplication of Vector3 and assign to itself
		inline Vector3& operator *= (const float s) {
			x *= s;
			y *= s;
			z *= s;
			return *this;
		}

		/// Overloaded division operator of Vector3 by a scalar
		inline const Vector3 operator / (const float s) const {
#ifdef _DEBUG
			if (fabs(s) < VERY_SMALL) {	// Checking to see if we are dividing by zero
				std::string errorMsg("Error! Dividing by nearly zero!");
				throw errorMsg;		// Throws an error message if we do
			}
#endif
			float r = 1.0f / s;
			return *this * r;
		}

		/// Overloaded division operator of Vector3 by a scalar and assignment onto itself
		inline Vector3& operator /= (const float s) {
#ifdef _DEBUG
			if (fabs(s) < VERY_SMALL) {	// Checking if dividing by zero
				std::string errorMsg("Error! Dividing by nearly zero!");
				throw errorMsg;		// Throws an error if you are
			}
#endif
			float r = 1.0f / s;
			return *this *= r;		// Multiplying by the fractional version of s
		}

		/// Utility to print the Vector3 components
		inline void print() {
			printf("%1.8f\t%1.8f\t%1.8f\n", x, y, z);
		}

		///
		/// Type conversion operators 
		///
		inline operator const float* () const {
			return static_cast<const float*>(&x);
		}

		inline operator float* () {
			return static_cast<float*>(&x);
		}

		/// Current work around to allow scalar vector3 multiplication with the scalar first 
		inline friend Vector3 operator * (const float s, const Vector3& v) {
			return v * s;
		}

		// Returns the length of this Vector3
		inline float magnitude() {
			return sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f));
		}

		// Returns the unit vector of this Vector3
		Vector3 normalized();

		// Makes this Vector3 have a magnitude of 1
		void Normalize();

		// Multiplies two Vector3 component-wise
		void Scale(const Vector3& v);

	};

	// Structure representing a 4D vector
	struct Vector4 : public Vector3 {
		float w;	// Inherits x, y, z from Vector3 

		/// Utility used to populate a Vector4
		inline void Load(float x_, float y_, float z_, float w_) {
			x = x_; y = y_; z = z_; w = w_;
		}

		/// Default constructor for Vector4
		inline explicit Vector4(float s = float(0.0)) {	// Default all components to zero unless a paramter is passed
			x = s; y = s; z = s; w = s;
		}

		/// A constuctor Vector4
		inline Vector4(float x_, float y_, float z_, float w_) {
			x = x_; y = y_; z = z_; w = w_;
		}

		/// A copy constructor
		inline Vector4(const Vector4& v) {
			x = v.x; y = v.y; z = v.z; w = v.w;
		}

		/// A constructor that takes a Vector3 components and sets w to 1.0f
		inline Vector4(const Vector3& v) {
			x = v.x; y = v.y; z = v.z; w = 1.0f;
		}

		/// 'Assignment Operator' Overloaded for Vector4
		inline Vector4& operator=(const Vector4& v) {
			x = v.x; y = v.y; z = v.z; w = v.w;
			return *this;
		}

		/// Set a Vector4 to negative of itself
		inline const Vector4 operator - () const {
			return Vector4(-x, -y, -z, -w);
		}

		/// In order to make Vector4 an array two overloaded operators are required

		/// This one allows Vector4 to be read as an array, const meaning it won't be changed
		inline const float& operator [] (int index) const {
			return *(&x + index);	// Returning a dereferenced address of the first component of Vector4 returns the lvalue at address
		}

		/// This overloaded operator allows Vector4 to written as ana array, hence it is not const
		inline float& operator [] (int index) {
			return *(&x + index);
		}

		/// Overloaded addition Vector4 operator
		inline const Vector4 operator + (const Vector4& v) const {
			return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
		}

		/// Overloaded subtraction Vector4 operator
		inline const Vector4 operator - (const Vector4& v) const {
			return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
		}

		/// Overloaded addition Vector4 operator and assignment to itself
		inline Vector4& operator += (const Vector4& v) {
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}

		/// Overloaded subtraction Vector4 operator and assignment to itself
		inline Vector4& operator -= (const Vector4& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}

		/// Overloaded multiplication Vector4 operator by a scalar value
		inline const Vector4 operator * (const float s) const {
			return Vector4(x * s, y * s, z * s, w * s);
		}

		/// Overloaded multiplication Vector4 by a scalar value and assignment onto itself
		inline Vector4& operator *= (const float s) {
			x *= s;
			y *= s;
			z *= s;
			w *= s;
			return *this;
		}

		/// Overloaded division operator Vector4 by a scalar value
		inline const Vector4 operator / (const float s) const {
#ifdef _DEBUG
			if (fabs(s) < VERY_SMALL) {	// Checking to see if you are dividing by zero
				std::string errorMsg("Error! Dividing by nearly zero!");
				throw errorMsg;		// Throws an error is you are dividing by zero
			}
#endif
			float r = 1.0f / s;
			return *this * r;
		}

		/// Overloaded division of Vector4 by scalar and assignment onto itself
		inline Vector4& operator /= (const float s) {
#ifdef _DEBUG
			if (fabs(s) < VERY_SMALL) {
				std::string errorMsg("Error! Dividing by nearly zero!");
				throw errorMsg;
			}
#endif
			float r = 1.0f / s;
			return *this *= r;
		}


		inline void print() {
			printf("%1.8f\t%1.8f\t%1.8f\t%1.8f\n", x, y, z, w);
		}

		///
		/// Type conversion operators 
		///
		inline operator const float* () const {
			return static_cast<const float*>(&x);
		}

		inline operator float* () {
			return static_cast<float*>(&x);
		}

		/// Current work around to allow scalar vector4 multiplication with the scalar first 
		inline friend Vector4 operator * (const float s, const Vector4& v) {
			return v * s;
		}

		// Calculates the length of this Vector4
		inline float magnitude() {
			return sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f) + pow(w, 2.0f));
		}

		// Returns the unit vector of this Vector4
		Vector4 normalized();
		
		// Sets the length of this Vector4 to 1
		void Normalize();

		// Multiplies two Vector4 component-wise
		void Scale(const Vector4& v);

	};

	// Class used to perform Vector oprations accross all Vector stuctures
	class VMath {

	public:
		///// ***** VECTOR2 ***** \\\\\

		// Calculates the dot product between two Vector2
		static float Dot(const Vector2& v1, const Vector2& v2);

		// Calculates the length between two Vector2
		static float Distance(const Vector2& v1, const Vector2& v2);

		// Calculates the angle between two Vector2 in radians
		static float Angle(const Vector2& v1, const Vector2& v2);
		
		// Linear Interpolation between starting vector2 and end vector2 at rate t
		// When t = 0 returns startPosition. 
		// When t = 1 returns end position. 
		// When t = 0.5 returns midway between start and end positions.
		static Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t);
		
		// Reflects a Vector2 on a Vector2 defined by its normal
		static Vector2 Reflect(const Vector2& v, const Vector2& n);



		///// ***** VECTOR3 ***** \\\\\

		// Calculates the normal Vector3
		static Vector3 normalize(const Vector3& v);
		
		// Calculates the dot product of two Vector3
		static float Dot(const Vector3& v1, const Vector3& v2);
		
		// Calculates the length between two Vector3
		static float Distance(const Vector3& v1, const Vector3& v2);
		
		// Calculates the angle between two Vector3
		static float Angle(const Vector3& v1, const Vector3& v2);
		
		// Linear Interpolation between starting vector3 and end vector3 at rate t
		// When t = 0 returns startPosition
		// When t = 1 returns end position
		// When t = 0.5 returns midway between start and end positions
		static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
		
		// Reflects a Vector3 on a Vector3 defined by its normal
		static Vector3 Reflect(const Vector3& v, const Vector3& n);
		
		// Calculates the cross product between two Vector3
		static Vector3 Cross(const Vector3& a, const Vector3& b);
		
		// Calculates Projection of one Vector3 onto another Vector3
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

	};

	// Class representing a column major 4x4 matrix
	class Matrix4 {
	// Shoutout and thank you to Scott
	/// These are the default vectors of the eye (camera) according to OpenGL and the right hand rule
	///								+Y   -Z
	///	                             |  /
	///   origin(0.0f,0.0f,0.0f);    | /
	///   up(0.0f,1.0f,0.0f);        |/_____+X
	///   forward(0.0f,0.0f,-1.0f);  /
	///                             /
	///                           +Z
	///

	/// Let's just make sure that all is clear about how this matrix is layed out. 

	/// How a matrix is really layed out is pretty much abitrary but we need to agree
	/// and the world has agreed (except for Microsoft) on the right-hand rule. 
	/// First, the 4x4 matrix is really just an array of 16 numbers.  
	/// We need to think of the array as a matrix in the following way
	/// 4x4 matrix - COLUMN MAJOR - The OpenGL, science, physics, mathematics and engineering way. 
	///	0  4  8  12        [0][0]  [1][0]  [2][0]  [3][0]   
	///	1  5  9  13  (or)  [0][1]  [1][1]  [2][1]  [3][1]
	///	2  6  10 14        [0][2]  [1][2]  [2][2]  [3][2]
	///	3  7  11 15        [0][3]  [1][3]  [2][3]  [3][3]
	private:
		float m[16];

	public:

		inline Matrix4& operator = (const Matrix4 &m_) {
			this->m[0] = m_[0]; this->m[1] = m_[1]; this->m[2] = m_[2]; this->m[3] = m_[3];
			this->m[4] = m_[4]; this->m[5] = m_[5]; this->m[6] = m_[6]; this->m[7] = m_[7];
			this->m[8] = m_[8]; this->m[9] = m_[9]; this->m[10] = m_[10]; this->m[11] = m_[11];
			this->m[12] = m_[12]; this->m[13] = m_[13]; this->m[14] = m_[14]; this->m[15] = m_[15];
			return *this;
		}


		inline explicit Matrix4(float x0, float x1, float x2, float x3,
			float y0, float y1, float y2, float y3,
			float z0, float z1, float z2, float z3,
			float w0, float w1, float w2, float w3) {
			m[0] = x0;   m[4] = y0;   m[8] = z0; m[12] = w0;
			m[1] = x1;   m[5] = y1;   m[9] = z1; m[13] = w1;
			m[2] = x2;   m[6] = y2;   m[10] = z2; m[14] = w2;
			m[3] = x3;   m[7] = y3;   m[11] = z3; m[15] = w3;
		}

		// Create the unit matrix probably the most common way of initializing a matrix
		inline explicit Matrix4(const float d = 1.0f) {
			if (d == 1.0f) {
				LoadIdentity();
			}
			else {
				m[0] = d;   m[4] = d;   m[8] = d;   m[12] = d;
				m[1] = d;   m[5] = d;   m[9] = d;   m[13] = d;
				m[2] = d;   m[6] = d;   m[10] = d;   m[14] = d;
				m[3] = d;   m[7] = d;   m[11] = d;   m[15] = d;
			}
		}


		// Creates the identity matrix
		inline void LoadIdentity() {
			m[0] = 1.0f;   m[4] = 0.0f;   m[8] = 0.0f;   m[12] = 0.0f;
			m[1] = 0.0f;   m[5] = 1.0f;   m[9] = 0.0f;   m[13] = 0.0f;
			m[2] = 0.0f;   m[6] = 0.0f;   m[10] = 1.0f;   m[14] = 0.0f;
			m[3] = 0.0f;   m[7] = 0.0f;   m[11] = 0.0f;   m[15] = 1.0f;
		}

		/// Multiply two 4x4 matricies. 
		inline const Matrix4 operator*(const Matrix4& n) const {
			/*Matrix4 result;
			for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
			result[i*4+j] = (m[0*4+j] * n[i*4 +0]) + (m[1*4+j] * n[i*4+1]) + (m[2*4+j] * n[i*4+2]) + (m[3*4+j] * n[i*4+3]);
			}
			}*/
			/// This approach is about 8 nanoseconds faster, not because I unrolled the loops but because of the constructor, ask me. 
			return Matrix4(
				(m[0 * 4 + 0] * n[0 * 4 + 0]) + (m[1 * 4 + 0] * n[0 * 4 + 1]) + (m[2 * 4 + 0] * n[0 * 4 + 2]) + (m[3 * 4 + 0] * n[0 * 4 + 3]),
				(m[0 * 4 + 1] * n[0 * 4 + 0]) + (m[1 * 4 + 1] * n[0 * 4 + 1]) + (m[2 * 4 + 1] * n[0 * 4 + 2]) + (m[3 * 4 + 1] * n[0 * 4 + 3]),
				(m[0 * 4 + 2] * n[0 * 4 + 0]) + (m[1 * 4 + 2] * n[0 * 4 + 1]) + (m[2 * 4 + 2] * n[0 * 4 + 2]) + (m[3 * 4 + 2] * n[0 * 4 + 3]),
				(m[0 * 4 + 3] * n[0 * 4 + 0]) + (m[1 * 4 + 3] * n[0 * 4 + 1]) + (m[2 * 4 + 3] * n[0 * 4 + 2]) + (m[3 * 4 + 3] * n[0 * 4 + 3]),
				(m[0 * 4 + 0] * n[1 * 4 + 0]) + (m[1 * 4 + 0] * n[1 * 4 + 1]) + (m[2 * 4 + 0] * n[1 * 4 + 2]) + (m[3 * 4 + 0] * n[1 * 4 + 3]),
				(m[0 * 4 + 1] * n[1 * 4 + 0]) + (m[1 * 4 + 1] * n[1 * 4 + 1]) + (m[2 * 4 + 1] * n[1 * 4 + 2]) + (m[3 * 4 + 1] * n[1 * 4 + 3]),
				(m[0 * 4 + 2] * n[1 * 4 + 0]) + (m[1 * 4 + 2] * n[1 * 4 + 1]) + (m[2 * 4 + 2] * n[1 * 4 + 2]) + (m[3 * 4 + 2] * n[1 * 4 + 3]),
				(m[0 * 4 + 3] * n[1 * 4 + 0]) + (m[1 * 4 + 3] * n[1 * 4 + 1]) + (m[2 * 4 + 3] * n[1 * 4 + 2]) + (m[3 * 4 + 3] * n[1 * 4 + 3]),
				(m[0 * 4 + 0] * n[2 * 4 + 0]) + (m[1 * 4 + 0] * n[2 * 4 + 1]) + (m[2 * 4 + 0] * n[2 * 4 + 2]) + (m[3 * 4 + 0] * n[2 * 4 + 3]),
				(m[0 * 4 + 1] * n[2 * 4 + 0]) + (m[1 * 4 + 1] * n[2 * 4 + 1]) + (m[2 * 4 + 1] * n[2 * 4 + 2]) + (m[3 * 4 + 1] * n[2 * 4 + 3]),
				(m[0 * 4 + 2] * n[2 * 4 + 0]) + (m[1 * 4 + 2] * n[2 * 4 + 1]) + (m[2 * 4 + 2] * n[2 * 4 + 2]) + (m[3 * 4 + 2] * n[2 * 4 + 3]),
				(m[0 * 4 + 3] * n[2 * 4 + 0]) + (m[1 * 4 + 3] * n[2 * 4 + 1]) + (m[2 * 4 + 3] * n[2 * 4 + 2]) + (m[3 * 4 + 3] * n[2 * 4 + 3]),
				(m[0 * 4 + 0] * n[3 * 4 + 0]) + (m[1 * 4 + 0] * n[3 * 4 + 1]) + (m[2 * 4 + 0] * n[3 * 4 + 2]) + (m[3 * 4 + 0] * n[3 * 4 + 3]),
				(m[0 * 4 + 1] * n[3 * 4 + 0]) + (m[1 * 4 + 1] * n[3 * 4 + 1]) + (m[2 * 4 + 1] * n[3 * 4 + 2]) + (m[3 * 4 + 1] * n[3 * 4 + 3]),
				(m[0 * 4 + 2] * n[3 * 4 + 0]) + (m[1 * 4 + 2] * n[3 * 4 + 1]) + (m[2 * 4 + 2] * n[3 * 4 + 2]) + (m[3 * 4 + 2] * n[3 * 4 + 3]),
				(m[0 * 4 + 3] * n[3 * 4 + 0]) + (m[1 * 4 + 3] * n[3 * 4 + 1]) + (m[2 * 4 + 3] * n[3 * 4 + 2]) + (m[3 * 4 + 3] * n[3 * 4 + 3]));
		}

		// Multipling a matrix by itself is probably the most commom
		// ("this" is the address of the matrix. "*this" de-references that address
		inline Matrix4& operator*=(const Matrix4& n) {
			*this = *this * n;
			return *this;
		}

		// Multiply a Vec4 by this matrix and return the resulting vector
		inline  Vector4 operator* (const Vector4& v) const {
			float x = v.x * m[0] + v.y * m[4] + v.z * m[8] + v.w * m[12];
			float y = v.x * m[1] + v.y * m[5] + v.z * m[9] + v.w * m[13];
			float z = v.x * m[2] + v.y * m[6] + v.z * m[10] + v.w * m[14];
			float w = v.x * m[3] + v.y * m[7] + v.z * m[11] + v.w * m[15];
			return Vector4(x, y, z, w);
		}

		/// Now I can use the structure itself as an array.
		/// When overloading the [] operator you need to declair one
		/// to read the array and one to write to the array. 
		///  Returns a const rvalue
		inline const float operator [] (int index) const {
			return *(m + index);
		}

		/// This one is for writing to the structure as if where an array 
		/// it returns a lvalue
		inline float& operator [] (int index) {
			return *(m + index);
		}

		inline  Vector3 operator* (const Vector3& v) const {
			float x = v.x * m[0] + v.y * m[4] + v.z * m[8] + 1.0f * m[12];
			float y = v.x * m[1] + v.y * m[5] + v.z * m[9] + 1.0f * m[13];
			float z = v.x * m[2] + v.y * m[6] + v.z * m[10] + 1.0f * m[14];
			float w = v.x * m[3] + v.y * m[7] + v.z * m[11] + 1.0f * m[15];
			return Vector3(x, y, z);
		}

		/// These allow me convert from type Matrix to const float * without issues
		inline operator float* () { return static_cast<float*>(&m[0]); }
		inline operator const float* () const { return static_cast<const float*>(&m[0]); }

		inline void print() const { /// Print them in column form (right-hand rule)
			printf("%1.8f %1.8f %1.8f %1.8f\n%1.8f %1.8f %1.8f %1.8f\n%1.8f %1.8f %1.8f %1.8f\n%1.8f %1.8f %1.8f %1.8f\n\n",
				m[0], m[4], m[8], m[12],
				m[1], m[5], m[9], m[13],
				m[2], m[6], m[10], m[14],
				m[3], m[7], m[11], m[15]);
		}


		/// These are a few esoteric funtions 
		/// Defines functions to access rows and columns of a Matrix4.
		inline Vector4 getColumn(int index) {
			return Vector4(m[4 * index + 0], m[4 * index + 1], m[4 * index + 2], m[4 * index + 3]);
		}
		inline Vector4 getRow(int index) {
			return Vector4(m[0 + index], m[4 + index], m[8 + index], m[12 + index]);
		}

	};

	// Class representing a column major 3x3 matrix
	class Matrix3 {

	private:
		float m[9];

	public:
		// Default Conststructor
		inline explicit Matrix3(float d = float(0.0)) {
			if (d == 0.0f)	// When the passed value is zero, lets create an identity matrix
				LoadIdentity();
			else {			// Otherwise populate the matrix with the value of d
				m[0] = d;	m[1] = d;	m[2] = d;
				m[3] = d;	m[4] = d;	m[5] = d;
				m[6] = d;	m[7] = d;	m[8] = d;
			}
		}

		// Utility to populate a Matrix3
		inline Matrix3(float xx, float xy, float xz,
			float yx, float yy, float yz,
			float zx, float zy, float zz) {
			m[0] = xx;	m[1] = xy; m[2] = xz;
			m[3] = yx; m[4] = yy; m[5] = yz;
			m[6] = zx; m[7] = zy; m[8] = zz;
		}

		// Utility to populate a Matrix3
		inline void Load(float xx, float xy, float xz,
			float yx, float yy, float yz,
			float zx, float zy, float zz) {
			m[0] = xx;	m[1] = xy;	m[2] = xz;
			m[3] = yx;	m[4] = yy;	m[5] = yz;
			m[6] = zx;	m[7] = zy;	m[8] = zz;
		}

		// In order to use Matrix3 as an array I requires two overloaded operators
		// One to read Matrix 3 as if it were an array 
		inline const float operator [] (int index) const {
			return *(m + index);
		}

		// One to write Matrix3 as an array
		inline float& operator [] (int index) {
			return *(m + index);
		}

		// Overloaded assignment operator
		inline Matrix3& operator=(const Matrix3& m_) {
			this->m[0] = m_[0];	this->m[1] = m_[1];	this->m[2] = m_[2];
			this->m[3] = m_[3];	this->m[4] = m_[4];	this->m[5] = m_[5];
			this->m[6] = m_[6];	this->m[7] = m_[7];	this->m[8] = m_[8];
			return *this;
		}

		// Load an identity matrix
		inline void LoadIdentity() {
			this->m[0] = 1.0f; this->m[1] = 0.0f; this->m[2] = 0.0f;
			this->m[3] = 0.0f; this->m[4] = 1.0f; this->m[5] = 0.0f;
			this->m[6] = 0.0f; this->m[7] = 0.0f; this->m[8] = 1.0f;
		}

		// Populate an Diagonal Matrix with a value
		inline void LoadIdentity(float d) {
			if (d == 0.0f)
				d = 1.0f;

			this->m[0] = d;	this->m[1] = 0.0f;	this->m[2] = 0.0f;
			this->m[3] = 0.0f;	this->m[4] = d;	this->m[5] = 0.0f;
			this->m[6] = 0.0f;	this->m[7] = 0.0f;	this->m[8] = d;
		}

		// Overloaded multiplication operator between two Matrix3 
		inline const Matrix3 operator * (const Matrix3& n) const {
			Matrix3 result;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					result[i * 3 + j] = (m[0 * 3 + j] * n[i * 3 + 0]) + (m[1 * 3 + j] * n[i * 3 + 1]) + (m[2 * 3 + j] * n[i * 3 + 2]);
				}
			}
			return result;
		}

		// Overloaded multiplication of two Matrix3 and asignment onto self
		inline Matrix3& operator *= (const Matrix3& n) {
			*this = *this * n;
			return *this;
		}

		// Overloaded multiplication operator between a Vector3 and Matrix3
		inline const Vector3 operator * (const Vector3& v) {
			Vector3 result;
			result.x = m[0] * v.x + m[1] * v.y + m[2] * v.z;
			result.y = m[3] * v.x + m[4] * v.y + m[5] * v.z;
			result.z = m[6] * v.x + m[7] * v.y + m[8] * v.z;
			return result;
		}


		// Extracts the inner 3x3 from a 4x4 matrix
		// using the assignment operator
		inline Matrix3& operator = (const Matrix4 &m_) {
			this->m[0] = m_[0]; this->m[1] = m_[1]; this->m[2] = m_[2];
			this->m[3] = m_[4]; this->m[4] = m_[5]; this->m[5] = m_[6];
			this->m[6] = m_[8]; this->m[7] = m_[9]; this->m[8] = m_[10];
			return *this;
		}

		// Extracts the inner 3x3 from a 4x4 matrix
		// using the constructor
		inline Matrix3(const Matrix4 &m_) {
			this->m[0] = m_[0]; this->m[1] = m_[1]; this->m[2] = m_[2];
			this->m[3] = m_[4]; this->m[4] = m_[5]; this->m[5] = m_[6];
			this->m[6] = m_[8]; this->m[7] = m_[9]; this->m[8] = m_[10];
		}

		// Print all the compoents of the assignment
		inline void print() {
			printf("%1.8f\t%1.8f\t%1.8f\n%1.8f\t%1.8f\t%1.8f\n%1.8f\t%1.8f\t%1.8f\n\n",
				m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8]);
		}


	};

	// Class used to perform Matrix operations
	class MMath {
	public:
		// See Note #5
		
		// Rotates this Matrix4 about any of the provided axes, and the given the degrees of rotation
		static Matrix4 Rotate(const float degrees, const float x, const float y, const float z);
		
		// Rotates this Matrix4 about any of the provided axes as a Vector3, and the given the degrees of rotation
		static Matrix4 Rotate(const float degrees, const Vector3 &axis);

		// Preforms uniform scaling, provided the scale factor on each axis
		static Matrix4 Scale(const float x, const float y, const float z);

		// Preforms uniform scaling, provided the scale factor on each axis as a Vector3
		static Matrix4 Scale(const Vector3& scale);

		// Translates this Matrix4 across the given direction components
		static Matrix4 Translate(const float x_, const float y_, const float z_);

		// Translates this Matrix4 across the given Vector3 direction components
		static Matrix4 Translate(const Vector3& translate_);
		
		// Geometric operation of Perspective projection
		static Matrix4 Perspective(const float fovy_, const float aspect_, const float zNear_, const float zFar_);

		// This creates a transform from Normalized Device Coordinates (NDC) to screen coordinates.
		static Matrix4 ViewportNDC(const int width_, const int height_);

		// This Orthographic function is a dimension reducing operatorion
		static Matrix4 Orthographic(const float xMin_, const float xMax_,
			const float yMin_, const float yMax_,
			const float zMin_, const float zMax_);


		static Matrix4 UnOrtho(const Matrix4& ortho);

		// Look At functions creating the eye, look at position and up position of a camera
		static Matrix4 LookAt(const float eyeX_, const float eyeY_, const float eyeZ_,
			const float atX_, const float atY_, const float atZ_,
			const float upX_, const float upY_, const float upZ_);

		// Look At functions creating the eye, look at position and up position of a camera
		static Matrix4 LookAt(const Vector3& eye, const Vector3& at, const Vector3& up);

		// Switchs the columns with the rows in the Matrix4
		static Matrix4 Transpose(const Matrix4& m);

		// Returns the inverse of a Matrix4
		static Matrix4 Inverse(const Matrix4& m);
	};

	// Structure representing the an Euler Angle
	struct Euler: public Vector3 {
		// The angles in the Euler Struct should never exceed 360.0
		// Representing a sequence of rotations
		// pitch, yaw, roll;

		// Some general protection/ memory management;
		//Euler(const Euler&) = delete;
		//Euler(const Euler&&) = delete;
		//Euler&operator=(const Euler&) = delete;
		//Euler&operator=(const Euler&&) = delete;

		// Default constructor for the Euler Struct
		inline explicit Euler(float a = float(0.0)) {
			x = a; y = a; z = a;
		}

		// A constructor populating the components of a Euler
		inline Euler(float pitch_, float yaw_, float roll_) {
			Load(pitch_, yaw_, roll_);
		}

		// Utility used to populate the components of a Euler
		inline void Load(float pitch_, float yaw_, float roll_) {
			x = pitch_; y = yaw_; z = roll_;
		}

		// A copy constructor
		inline Euler(const Euler& e) {
			x = e.x; y = e.y; z = e.z;
		}

		// An assignment operator
		inline Euler& operator= (const Euler& e) {
			x = e.x; y = e.y; z = e.z;
			return *this;
		}
		
	};

	// Structure representing a Quaternion 
	struct Quaternion : public Vector4 {

		//Quaternion(const Quaternion&) = delete;
		//Quaternion(const Quaternion&&) = delete;
		//Quaternion&operator=(const Quaternion&) = delete;
		//Quaternion&operator=(const Quaternion&&) = delete;

		// Default constructor for the Quaternion class
		inline explicit Quaternion(float s = float(0.0)) {
			x = s; y = s; z = s; 
			(s == 0.0f) ? w = 1.0f : w = s;
		}

		// A constructor that populates the components of the Quaternion
		inline Quaternion(float x_, float y_, float z_, float w_) {
			Load(x_, y_, z_, w_);
		}

		// Utility to populate the Quaternion components
		inline void Load(float x_, float y_, float z_, float w_) {
			x = x_; y = y_; z = z_; w = w_;
		}

		// A constructor taking the axis and degrees of rotation
		inline Quaternion(const Vector3& axis, float degrees) {
			degrees *= DEGREES_TO_RADIANS;
			w = cos(degrees / 2.0f);
			x = axis.x * sin(degrees / 2.0f);
			y = axis.y * sin(degrees / 2.0f);
			z = axis.z * sin(degrees / 2.0f);
		}

		// A copy constructor
		inline Quaternion(const Quaternion& q) {
			x = q.x; y = q.y; z = q.z; w = q.w;
		}
		
		// An assignment operator
		inline Quaternion& operator=(const Quaternion& q) {
			x = q.x; y = q.y; z = q.z; w = q.w;
			return *this;
		}

		// **WIP:: Preforms the addition between two quaternions
		inline const Quaternion operator + (const Quaternion& q) const {
			return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
		}

		// Preforms the multiplication between two quaternions
		inline const Quaternion operator * (const Quaternion& q) const {
			Vector3 v1(x, y, z);
			Vector3 v2(q.x, q.y, q.z);

			float w_ = (w * q.w) - (x * q.x + y * q.y + z * q.z);
			Vector3 v = (w * v2) + (q.w * v1) + (VMath::Cross(v1, v2));

			return Quaternion(v.x, v.y, v.z, w_);
		}
		
		// Preforms the multiplication between a scalar and a quaternion
		inline const Quaternion operator * (const float t) const {
			return Quaternion(x * t, y * t, z * t, w * t);
		}

		/// Current work around to allow scalar quaternion multiplication with the scalar first 
		inline friend Quaternion operator * (const float t, const Quaternion& q) {
			return q * t;
		}

		// Preforms the multiplication between a scalar and a quaternion and assignment onto it self
		inline Quaternion& operator *= (const float t) {
			x *= t;
			y *= t;
			z *= t;
			w *= t;
			return *this;
		}

		// Returns the conjugate of this Quaternion, therfore reversing the Vector component of the Quaternion
		inline Quaternion conjugated() {
			return Quaternion(-x, -y, -z, w);
		}

		// Returns the unit Quaternion of this Quaternion
		Quaternion normalized();

		// Returns the representation of this Quaternion as Euler Angles
		Euler euler();

		inline void print() {
			printf("%1.8f\t%1.8f\t%1.8f\t%1.8f\n", x, y, z, w);
		}

	};
	
	// Class used to perform Quarternion Math
	class QMath {

	public:

		// Calculates the normalized conjugate of a Quaternion
		static Quaternion Inverse(const Quaternion& q);

		// Returns the conjugate of the passed Quaternion
		static Quaternion Conjugate(const Quaternion& q);

		// Calculates the unit quaternion
		static Quaternion Normalize(const Quaternion& q);

		// Calculates the dot product between two quaternions
		static float Dot(const Quaternion& q1, const Quaternion& q2);

		// **WIP::Calculates the angle between two Quaternions
		static float Angle(const Quaternion& q1, const Quaternion& q2);

		// Calculates the rotation of a Vector3 using the passed Quaternion
		static Vector3 Rotate(const Quaternion& q, const Vector3& v);

		// **WIP::Linear Interpolation between starting Quaternion and end Quaternion at rate t
		// When t = 0 returns startPosition
		// When t = 1 returns end position
		// When t = 0.5 returns midway between start and end positions
		static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);
		
		// **WIP:: Calculates the Spherical Linear Interpolation between the passed starting point and end position at the rate of t
		// If the angle between the two Vector3 is less than 10 degrees than we use linear interpolation
		// When t = 0 returns startPosition 
		// When t = 1 returns end position 
		// When t = 0.5 returns midway between start and end positions
		static Quaternion SLerp(const Quaternion& q1, const Quaternion& q2, float t);
		
	};

	// Class used to perform conversion between Matrices, Quaternions and Euler Angles
	class Convert {
	public:

		// Converting the representation of a Euler to a Matrix4
		static Matrix4 EulerToMatrix(const Euler& e);

		// Converting the representation of a Matrix4 to an Euler
		static Euler MatrixToEuler(const Matrix4& m);

		// Converting a Quaternion into a 4x4 Matrix
		static Matrix4 QuaternionToMatrix(const Quaternion& q);

		// Converting a Matrix into a Quaternion
		static Quaternion MatrixToQuaternion(const Matrix4& m);

		// Converting an Euler(Y, Z, X) into a Quaternion
		static Quaternion EulerToQueaternion(const Euler& e);

		// Converting a Quaternion into an Euler(Y, Z, X)
		static Euler QuaternionToEuler(const Quaternion& q);

	};
}


#endif

// Note 1:
/// When working with arrays be careful... an array can be defined to have a number of blocks but you can override that and pop the stack either way

// Note 2:
/// Numbers will not be exactly what you want them to be 3.7f may be 3.6999988f, so be aware

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