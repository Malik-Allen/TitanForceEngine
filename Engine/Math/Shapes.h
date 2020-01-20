#ifndef SHAPES_H
#define SHAPES_H

#include "Math.h"


namespace SHAPES {

	// Structure used to represent a Ray which has a origin and direction using Vector3
	struct Ray {
		MATH::Vector3 origin;
		MATH::Vector3 direction;

		//Ray(const Ray&) = delete;
		//Ray& operator=(const Ray&) = delete;
		Ray(const Ray&&) = delete;
		Ray& operator=(const Ray&&) = delete;

		// A constructor for Ray
		inline Ray(const MATH::Vector3& origin_, const MATH::Vector3& direction_) {
			origin = origin_;
			direction = direction_;
		}

		// Calculates the current position(Vector3) of Ray at legnth t
		// Uses the direction's unit vector
		inline MATH::Vector3 CurrentPosition(float t) {
			return origin + direction * t;
		}

		// A copy constructor
		inline Ray(const Ray& r) {
			origin = r.origin;
			direction = r.direction;
		}

	};

	// Structure used to represent a Ray which has a origin and direction using Vector2
	struct Ray2D {
		//Ray2D(const Ray2D&) = delete;
		//Ray2D& operator=(const Ray2D&) = delete;
		Ray2D(const Ray2D&&) = delete;
		Ray2D& operator=(const Ray2D&&) = delete;

		MATH::Vector2 origin;
		MATH::Vector2 direction;

		// A Ray2D constructor
		inline Ray2D(const MATH::Vector2& origin_, const MATH::Vector2& direction_) {
			origin = origin_;
			direction = direction_;
		}

		// Calculates the current position(Vector2) of Ray2D at length t
		// Uses the direction's unit vector
		inline MATH::Vector2 CurrentPosition(float t) {
			return origin + direction * t;
		}

		// Copy constructor
		inline Ray2D(const Ray2D& r) {
			origin = r.origin;
			direction = r.direction;
		}
	};

	struct Plane : public MATH::Vector3 {
		float d; // Plane inherits x, y, and z components from Vector3

		// A constructor for Plane
		inline Plane(float x_, float y_, float z_, float d_) {
			x = x_; y = y_; z = z_; d = d_;
		}

		// Utility to load the components of Plane
		inline void Load(float x_, float y_, float z_, float d_) {
			x = x_; y = y_; z = z_; d = d_;
		}

		/// A 'safe' default contructor for the Plane 
		inline explicit Plane(float s = float(0.0)) {
			Load(s, s, s, s);
		}

		/// A constructor for Plane that uses 3 Vector3 to define the Plane
		Plane(const MATH::Vector3& v0, const MATH::Vector3& v1, const MATH::Vector3& v2);

		// Convert this plane into a normalized plane
		void normalize();

		// Calculates the position at which an intersection between this Plane and Ray takes place
		Vector3 IntersecPoint(const Ray& r);

		// Returns the flipped direction of this plane
		Plane flipped();

		// Flips the direction of this plane
		inline void Flip() {
			x = -x; y = -y; z = -z; // d= -d;
		}

		inline void print() {
			printf("%1.8f\t%1.8f\t%1.8f\t%1.8f\n", x, y, z, d);
		}

	};

	struct Sphere : public MATH::Vector3 {
		float r;

		// A Sphere constructor
		inline Sphere(float x_, float y_, float z_, float r_) {
			x = x_; y = y_; z = z_; r = r_;
		}

		// A constructor that uses passed Vector3 for the Sphere's location
		inline Sphere(const MATH::Vector3& v, float r_) {
			x = v.x; y = v.y; z = v.z; r = r_;
		}

		/// Utility to load the sphere position and radius
		inline void Load(float x_, float y_, float z_, float r_) {
			x = x_; y = y_; z = z_; r = r_;
		}

		inline explicit Sphere(float s = float(0.0)) {
			Load(s, s, s, s);
		}

		inline void print() {
			printf("%1.8f\t%1.8f\t%1.8f\t%1.8f\n", x, y, z, r);
		}

	};

	struct Box {

		//Box(const Box&) = delete;
		//Box& operator=(const Box&) = delete;
		Box(const Box&&) = delete;
		Box& operator=(const Box&&) = delete;

		float rx, ry, rz;
		Plane plane[6];

		/// Default constructor to populate
		inline explicit Box(float s = float(1.0)) {
			rx = s; ry = s; rz = s;
			plane[0].Load(1.0f, 0.0f, 0.0f, 0.0f);
			plane[1].Load(0.0f, 1.0f, 0.0f, 0.0f);
			plane[2].Load(0.0f, 0.0f, 1.0f, 0.0f);
			plane[3].Load(1.0f, 0.0f, 0.0f, -rx);
			plane[4].Load(0.0f, 1.0f, 0.0f, -ry);
			plane[5].Load(0.0f, 0.0f, 1.0f, -rz);
		}

		// Utility to populate the dimensions of this Box
		Box(float width, float height, float depth) {
			rx = width; ry = height; rz = depth;
			plane[0].Load(1.0f, 0.0f, 0.0f, 0.0f);
			plane[1].Load(0.0f, 1.0f, 0.0f, 0.0f);
			plane[2].Load(0.0f, 0.0f, 1.0f, 0.0f);
			plane[3].Load(1.0f, 0.0f, 0.0f, -rx);
			plane[4].Load(0.0f, 1.0f, 0.0f, -ry);
			plane[5].Load(0.0f, 0.0f, 1.0f, -rz);
		}

		// A copy constructor
		inline Box(const Box& b) {
			rx = b.rx;
			ry = b.ry;
			rz = b.rz;
			plane[0] = b.plane[0];
			plane[1] = b.plane[1];
			plane[2] = b.plane[2];
			plane[3] = b.plane[3];
			plane[4] = b.plane[4];
			plane[5] = b.plane[5];
		}


		inline void print() {
			printf("Width: %1.8f\nHeight: %1.8f\nDepth: %1.8f\n\n", rx, ry, rz);
		}


	};

	// Class used to perform operations between Shapes and Vector3
	class SMath {
	public:
		// Calculates the projections of one Vector3 onto a Plane's normal
		static MATH::Vector3 ProjectOnPlane(const MATH::Vector3& v, const Plane& p);

		// Calculates the distance between a given point (Vector3) and Plane
		static float Distance(const MATH::Vector3& v, const Plane& p);

		// Translates Plane by given Vector3
		static Plane Translate(const Plane& p, const MATH::Vector3& v);
	};

}

#endif

