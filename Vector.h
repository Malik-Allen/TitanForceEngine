#ifndef VECTOR_H
#define VECTOR_H
#include <string> 
#include "Debug.h"

/// Vector Header Capabilities Include:
/// Operator overides,
///	All basic vector math interactions, subttaction, multiplication, division, etc.
/// Plane, Sphere, and Ray definitions
/// Vector 3 definitions are followed by Vector 4 -- Feb 9, 2019

// Future iterations include:
// Sphere and ray intersection detection
// Cube of 6 plane and ray intersection detection
// Primative shapes -- Feb 13, 2019

namespace  MATH {

//Instead of dividing by zero, which return a fatal error
//Use a very small number

#ifndef VERY_SMALL
#define VERY_SMALL 1.0e-7f
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#ifndef DEGREES_TO_RADIANS
#define DEGREES_TO_RADIANS (M_PI / 180.0f)
#endif	

#ifndef RADIANS_TO_DEGREES
#define RADIANS_TO_DEGREES (180.0F / M_PI)
#endif

#ifndef EARTH_GRAVITY
#define EARTH_GRAVITY Vec3(0.0f, -9.8f, 0.0f)
#endif

#ifndef GRAVITATIONAL_CONSTANT
#define GRAVITATIONAL_CONSTANT 1.0f
#endif

	struct Vec3 {

		float  x, y, z;

		inline void Load(float newX, float newY, float newZ) {
			x = newX; 
			y = newY; 
			z = newZ;
		}

		//Vector Constructor using float zero
		inline Vec3(float scalar = float(0.0)) { //Default will set all values to zero
			Load(scalar, scalar, scalar);
		}

		// Load the values of x, y, z onto the vector
		inline Vec3(float newX, float newY, float newZ) {
			Load(newX, newY, newZ);
		}

		//A copy constructor
		inline Vec3(const Vec3& newVector) {
			Load(newVector.x, newVector.y, newVector.z);
		}

		////////////////////////////////////////////////////////////////
		/*Mathematical Functionality for Vec3*/
		////////////////////////////////////////////////////////////////////

		//Returns the magnitude of the vector
		inline float Magnitude() {
			return sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f));
		}

		//Returns the Dot Product of this vector * (otherVector)
		inline float DotProduct(Vec3& otherVec) {
			return (x * otherVec.x) + (y * otherVec.y) + (z * otherVec.z);
		}

		// Returns the angle between two vectors in radians
		// Remember the vector's position relative to one another
			//i.e Taking the negative of one of the vectors
		inline float AngleBetween(Vec3& otherVec) {
			return acos(this->DotProduct(otherVec) / (this->Magnitude() * otherVec.Magnitude()));
		}

		// Returns this vector projected on the passed vector
		inline Vec3 ProjectedOn(Vec3& otherVec) {

			Vec3 otherNormalVec = otherVec.GetNormalized();
			float scalar = this->DotProduct(otherNormalVec) / otherNormalVec.DotProduct(otherNormalVec);
			
			return *otherNormalVec * scalar;
		}
		// In this process you take the normal of the vector you are projecting onto
		// Take the dot product between this vector and the normalazied vector
		// Then it is divided by the normal vector dot normal vector
		// Finally the resultant scalar magnitude is multiplied with the normal vector to give you the projection vector


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Returns the cross product between this vector and the passed vector
		//If returned vector is zero than these two vectors are parallel
		inline Vec3 Cross(Vec3& otherVec) {
			Vec3 crossProduct;
			crossProduct.x = y * otherVec.z - z * otherVec.y;
			crossProduct.y = -(x * otherVec.z - z * otherVec.x);
			crossProduct.z = x * otherVec.y - y * otherVec.x;

			return crossProduct;
		}

		//Rotates the vector about the z-axis
		inline void RotateZ(float angleInDeg) {
			float angleInRad = angleInDeg * DEGREES_TO_RADIANS;

			float tempX = x * cos(angleInRad) - y * sin(angleInRad);
			float tempY = x * sin(angleInRad) - y * cos(angleInRad);

			x = tempX;
			y = tempY;
		}

		//Sets the vector's magnitude to one
		inline void Normalize() {
			float mag = sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f));

			x /= mag;
			y /= mag;
			z /= mag;
		}

		//Returns the Normalized Vector as a Vec3
		inline Vec3 GetNormalized() {
			float mag = sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f));

			return Vec3((x /= mag), (y /= mag), (z /= mag));
		}


		//Returns the magnitude of the distance between two vectors as a float
		inline float Distance(Vec3& otherVec) {
			Vec3 tempVec;
			tempVec.Load(x - otherVec.x, y - otherVec.y, z - otherVec.z);
			return tempVec.Magnitude();
		}

		//Interperlates between start position and end position. 
		//When t = 0 returns startPosition. 
		//When t = 1 returns end position. 
		//When t = 0.5 returns midway between start and end positions.
		inline void Lerp(Vec3& endPosition, float t) {
			Vec3 dist;
			dist.Load(endPosition.x - x, endPosition.y - y, endPosition.z - z);
			dist *= t;

			x += dist.x;
			y += dist.y;
			z += dist.z;
		}
		/*Using the Lerp function*/
		/*There are more than one type of way of making a lerp*/
		////////////////////////////////////////////////////

		//float currentLerpTime = 0.0f;
		//float lerpTime = 1.0f;
		//float speed = Speed of the lerp!;
		//if(currentLerpTime >= lerpTime)
		//	currentLerpTime = lerpTime;
		//float perc = (currentLerpTime * speed) / lerpTime;
		//startPosition.Lerp(endPosition, perc); 

		//BONUS: A boolean can switch the Lerp on and off when it is complete OR switch case works well with lerping
		////////////////////////////////////////////////////

		/*Quadratic Eqxn*/
		//Requires a bool in addition to the a, b, and c constants
		//Bool determines where the reseult is plus(true) or minus(false)
		inline float Quadratic(float a, float b, float c, bool returnPositive) {

			float quadCheck = b * b - (4 * a * c);

			if (quadCheck < 0.0f) {
				quadCheck = VERY_SMALL; 
				Debug::Error("Cannot find the square root of a negative number", __FILE__, __LINE__);
				return quadCheck;
			}
			else if (returnPositive) {
				return -b + sqrt(quadCheck / 2.0f * a);
			}
			else {
				return -b - sqrt(quadCheck / 2.0f * a);
			}
		}

		///////////////////////////////////////////////////////////
		/// Operator overloads (see note 1 at the end of this file)
		///////////////////////////////////////////////////////////

		// An assignment operator   
		inline Vec3& operator = (const Vec3& otherVec) {
			Load(otherVec.x, otherVec.y, otherVec.z);
			return *this;
		}

		/*MORE CLARITY NEED FOR ARRAY IMPLEMENTATION*/
		/// Now we can use the Vec3 like an array but we'll need two overloads
		inline const float operator [] (int index) const {  /// This one is for reading the Vec3 as if where an array
			return *(&x + index);
		}

		inline float& operator [] (int index) {	/// This one is for writing to the Vec3 as if where an array.  
			return *(&x + index);					/// See note 2 at the end of this file about lvalues and rvalues
		}


		//Addition of two Vec3s
		inline const Vec3 operator + (const Vec3& otherVec) const { //Set to const to make sure the Vec3 address will not be changed
			return Vec3(x + otherVec.x, y + otherVec.y, z + otherVec.z);
		}

		//Add a Vec3 to itself
		inline Vec3& operator += (const Vec3& otherVec) {
			x += otherVec.x;
			y += otherVec.y;
			z += otherVec.z;
			return *this;
		}

		//Return the negative of a Vec3
		inline const Vec3 operator - () const {
			return Vec3(-x, -y, -z);
		}

		// Subtraction of anaother Vec3
		inline const Vec3 operator - (const Vec3& otherVec) const {
			return Vec3(x - otherVec.x, y - otherVec.y, z - otherVec.z);
		}

		//Subtract a Vec3 from itself
		inline Vec3& operator -= (const Vec3& otherVec) {
			x -= otherVec.x;
			y -= otherVec.y;
			z -= otherVec.z;
			return *this;
		}

		//Multiply a vector by a scalar value
		inline const Vec3  operator * (const float scalar) const {
			return Vec3(scalar * x, scalar * y, scalar * z);
		}


		/// Multiply a scaler by a Vec3  - Ha! It's the scalar first then the Vec3
		/// Overloaded and a friend, ouch! It's the only way to make it work with a scalar first.
		/// Friends are tricky, look them up. 
		inline friend Vec3 operator * (const float scalar, const Vec3& otherVec) {
			return otherVec * scalar;
		}

		//Multiplying a Scalar by a Vector and assigning it to itself
		inline Vec3& operator *= (const float scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}


		//Division by a scalar value
		inline const Vec3 operator / (const float scalar) const {
#ifdef _DEBUG  //If in DEBUG MODE check if the Scalar is less VERY SMALL
			if (fabs(scalar) < VERY_SMALL) {
				std::string errorMsg("Divide by nearly zero! ");
				throw errorMsg;
			}
#endif
			float r = 1.0f / scalar;
			return *this * r;
		}

		//Division of a scalar into a Vector and assigned to itself
		inline Vec3& operator /= (const float scalar) {
#ifdef _DEBUG  // Double checking that the scalar is not zero! 
			if (std::fabs(scalar) < VERY_SMALL) {
				std::string errorMsg("Divide by nearly zero! ");
				throw errorMsg;
			}
#endif // DEBUG
			float r = 1.0f / scalar;
			*this *= r;
			return *this;
		}

		inline void print() {
			printf("%f %f %f\n", x, y, z);
		}

		/*TYPE CONVERSIONS*/
		///
		/// Type conversion operators 
		///
		inline operator const float* () const {
			return static_cast<const float*>(&x);
		}

		inline operator float* () {
			return static_cast<float*>(&x);
		}

	};

	/*Vec4 Definition*/
	//Inherits Vec3 Class
	//Can be used Vec4 as a SubRoutine of Vec3 in some cases

	struct Vec4 : public Vec3 {
		// float x, y and z components are inherited from Vec3

		float  w;

		// Vec4 Constructors
		inline Vec4(float scalar = float(0.0)) { 
			x = scalar;
			y = scalar; 
			z = scalar; 
			w = scalar; 
		}
		inline Vec4(float newX, float newY, float newZ, float newW) { 
			x = newX; 
			y = newY; 
			z = newZ; 
			w = newW; 
		}
		inline Vec4(const Vec4& otherVec) { //Assigns Vec4 on instantiation
			x = otherVec.x;
			y = otherVec.y;
			z = otherVec.z;
			w = otherVec.w;
		}

		inline Vec4(const Vec3& otherVec) { //Assigning Vec3 to Vec4 with w = 1.0f
			x = otherVec.x;
			y = otherVec.y;
			z = otherVec.z;
			w = 1.0f;
		}

		//Returns the magnitude of this Vec4 as a float
		inline float Magnitude() {
			return sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f) + pow(w, 2.0f));
		}

		// Normalizes this Vec4
		inline void Normalize() {
			float mag = sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f) + pow(w, 2.0f));

			x /= mag;
			y /= mag;
			z /= mag;
			w /= mag;
		}

		//Returns the normal of this vector as a Vec4
		inline Vec4 GetNormalized() {
			float mag = sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f) + pow(w, 2.0f));

			return Vec4((x /= mag), (y /= mag), (z /= mag), (w /= mag));
		}

		//Returns the magnitude of the vector as float
		inline float DotProduct(Vec4& otherVec) {
			return (x * otherVec.x + y * otherVec.y + z * otherVec.z + w * otherVec.w);
		}
		//Assignment operator
		inline Vec4& operator = (const Vec4& newVec) {
			x = newVec.x;
			y = newVec.y;
			z = newVec.z;
			w = newVec.w;
			return *this;
		}
		/*CLARIFY THIS!!*/
		// Creating Vec4 Array is possible
		inline float& operator [] (int index) {
			return *(&x + index);
		}
		inline const float operator [] (int i) const {
			return *(&x + i);
		}

		//Addition of a Vec4 
		inline Vec4 operator + (const Vec4& otherVec) const {
			return Vec4(x + otherVec.x, y + otherVec.y, z + otherVec.z, w + otherVec.w);
		}

		//Adding a Vec4 to itself
		inline Vec4& operator += (const Vec4& otherVec) {
			x += otherVec.x;
			y += otherVec.y;
			z += otherVec.z;
			w += otherVec.w;
			return *this;
		}

		//Returning the negative of a Vec4
		inline Vec4 operator - () const {
			return Vec4(-x, -y, -z, -w);
		}

		//Subtraction of a Vec4
		inline Vec4 operator - (const Vec4& otherVec) const {
			return Vec4(x - otherVec.x, y - otherVec.y, z - otherVec.z, otherVec.w - w);
		}

		//Subtraction of a Vec4 from itself 
		inline Vec4& operator -= (const Vec4& otherVec) {
			x -= otherVec.x;
			y -= otherVec.y;
			z -= otherVec.z;
			w -= otherVec.w;
			return *this;
		}

		// Multiplication of Scalar to a Vec4
		inline Vec4 operator * (const float scalar) const {
			return Vec4(scalar * x, scalar * y, scalar *z, scalar * w);
		}

		//Multiplication of a Scalar and assigning it to itself
		inline Vec4& operator *= (const float scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
		}

		//Overloading the operator 
		friend Vec4 operator * (const float scalar, const Vec4& otherVec) {
			return otherVec * scalar;
		}


		inline Vec4 operator / (const float scalar) const {
#ifdef DEBUG 
			if (std::fabs(scalar) < VERY_SMALL) {
				std::string errorMsg("Divide by nearly zero! ");
				throw errorMsg;
			}
#endif
			float r = 1.0f / scalar;
			return *this * r;
		}

		inline Vec4& operator /= (const float scalar) {
#ifdef _DEBUG 
			if (std::fabs(scalar) < VERY_SMALL) {
				std::string errorMsg("Divide by nearly zero! ");
				throw errorMsg;
			}
#endif // DEBUG

			float r = 1.0f / scalar;
			*this *= r;
			return *this;
		}

		inline void print() {
			printf("%f %f %f %f\n", x, y, z, w);
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

	};

	// A Ray is defined by its starting point and direction
	struct Ray { 

		Vec3 startPosition, direction;

		inline Ray(Vec3 newStartPosition, Vec3 newDirection) {
			startPosition = newStartPosition;
			direction = newDirection;
		}

		//Returns the current position of the Ray at magnitude t
		inline Vec3 CurrentPosition(float t) {
			return startPosition + direction * t;
		}

		inline Ray& operator=(const Ray& newRay) {
			startPosition = newRay.startPosition;
			direction = newRay.direction;

			return *this;
		}
	};


	/// A solution to the circular dependency problem.  
	/// This is what is called a "forward declaration."
	/// See note 3 at the bottom of this page.
	class VMath;

	///  float x,y,z; came from vector as the normal to the surface + distance to the surface (d) 
	struct Plane : public Vec3 {
		float d;

		/// Here's a set of constructors
		inline Plane(float _s = 0.0f) {
			x = _s;
			y = _s;
			z = _s;
			d = _s;
		}
		inline Plane(float _x, float _y, float _z, float _d) {
			x = _x;
			y = _y;
			z = _z;
			d = _d;
		}

		inline Plane(const Plane& v) {
			x = v.x;
			y = v.y;
			z = v.z;
			d = v.d;
		}

		/// This creates a normalized equation of a plane.
		inline Plane(const Vec3& v0, const Vec3& v1, const Vec3& v2) {
			Vec3 a = v1 - v0;
			Vec3 b = v2 - v0;
			Vec3 n = Vec3(a.y * b.z - a.z * b.y, /// This is the cross product
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x);
			float magnitude = float(sqrt(n.x * n.x + n.y * n.y + n.z * n.z)); /// normalize

			Plane(x / magnitude, y / magnitude, z / magnitude, a.x * b.x + a.y * b.y + a.z * b.z);
		}

		/// Convert this plane into a normalized plane
		inline void Normalize() {
			float a = sqrt(x*x + y * y + z * z);
			x /= a;
			y /= a;
			z /= a;
			d /= a;
		}


		// Returns the intersection point between this plane and passed Ray
		// Returns zero if Ray is on the plane, resulting in infinite intersection points
		// Returns zero if the Plane and Ray do not intersect and are parallel
		inline Vec3 IntersectPoint(Ray r) {

			Plane normalizedPlane(x, y, z, d);
			float t;

			normalizedPlane.Normalize();

			if (normalizedPlane.DotProduct(r.direction) != 0.0f) {
				t = ((-normalizedPlane.DotProduct(r.startPosition))) / normalizedPlane.DotProduct(r.direction) * -1.0f;
				return r.CurrentPosition(t);
			}
			else if (normalizedPlane.DotProduct(r.startPosition) + -normalizedPlane.DotProduct(r.startPosition) == 0.0f || 
						normalizedPlane.DotProduct(r.startPosition) + -normalizedPlane.DotProduct(r.startPosition) == VERY_SMALL) {
				Debug::Warning("The passed ray is on the plane, infinite intersection points", __FILE__, __LINE__);
				return Vec3(0.0f);
			}else { 
				Debug::Warning("This plane does not intersect with the ray passed and is parallel to the ray", __FILE__, __LINE__);
				return Vec3(0.0f);
			}
		}

		inline void print() {
			printf("%f %f %f %f\n", x, y, z, d);
		}
	};

	/// Just some extra stuff for fun

	///		A Sphere could be thought of as a just a center point (x,y,z) 
	///		comes from Vec3 then just add a radius
	struct Sphere : public Vec3 {
		float r;
		Sphere(float s = 0.0f) { x = s; y = s; z = s; r = s; }

		Sphere(float _x, float _y, float _z, float _r) {
			x = _x; y = _y; z = _z; r = _r;
		}

		inline Sphere(const Sphere& v) {
			x = v.x;  y = v.y;  z = v.z; r = v.r;
		}

		inline void print() {
			printf("%f %f %f %f\n", x, y, z, r);
		}
	};

}

#endif
