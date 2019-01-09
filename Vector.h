#ifndef VECTOR_H
#define VECTOR_H
#include <string> // Used to pass error messages
/*Vector 3 Definitions followed by Vector 4*/

namespace  MATH {

/*IMPORTANT*/
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

		//Vector Constructors
		inline Vec3(float scalar = float(0.0)) { //Default will set all values to zero
			Load(scalar, scalar, scalar);
		}

		inline Vec3(float newX, float newY, float newZ) {
			Load(newX, newY, newZ);
		}

		//A copy constructor
		inline Vec3(const Vec3& newVector) {
			Load(newVector.x, newVector.y, newVector.z);
		}

		/*Mathematical Functionality for Vec3*/

		//Returns the magnitude of the vector
		inline float Magnitude() {
			return sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f));
		}

		//Returns the Dot Product of this vector * (otherVector)
		inline float DotProduct(Vec3& otherVec) {
			return (x * otherVec.x) + (y * otherVec.y) + (z * otherVec.z);
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

		//Returns the magnitude of the distance between two vectors
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
		/// Important: See note 3.
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
		inline void normalize() {
			float a = sqrt(x*x + y * y + z * z);
			x /= a;
			y /= a;
			z /= a;
			d /= a;
		}
		inline void print() {
			printf("%f %f %f %f\n", x, y, z, d);
		}
	};

}

#endif


/*** Note 1.
I know, I hate operator overloading as a general rule but a few make sense!! Just be careful and
consistent. In the following code, I will overload many operators. I don't believe in
overloading when the operator is less than obvious.
For example, in this class, the relational operators (== != < > <= >=) might mean "in relation
to their magnitude or direction" I'm just not sure. Just write a function to do that and don't make
me guess what the operator might mean. Use the idea of "Least Astonishment" don't surprise me,
or shock me or anything else that will piss me or others off-SSF
***/

/*** Note 2.
In straight C programming, lvalues and rvalues literially means left and right assignments.  For example,
int x = 5; x is the lvalue and 5 is the rvalue. Easy. In C++ it get a bit more tricky. The modern idea is
rvalues are temporary vaules residing in the registers of the CPU. lvalues are actual memory locations.
In the code:
	inline float& operator [] ( int index ) {
		return *(&x + index);
	}
To read this precicely, &x is the address of the x variable (the first in the list of x,y,z) add to that
address the index as if it were an array. Then *(&x + index) dereferences that address back into the
object, the float& reference returns it as an address and thus an lvalue.
***/

/*** Note 3.
The issue here is that I need to use functions in Vector from VMath which require Vector but VMath
also requires Vector - this is a classic circular dependency problem or sometimes known as mutual recursion.
To solve this it requires a forward declaration.  A "forward declaration" is the declaration of a class for which
the you have not yet given a complete definition of (whateverClass.h). To do this all you need is the statement:
"class VMath;"  This warns the compiler that I will be using VMath in Vector; HOWEVER, in this case, it won't work because
I'm using the vector.h as a totally inlined set of functions - sometimes your just screwed
***/