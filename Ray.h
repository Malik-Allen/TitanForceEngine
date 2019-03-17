#ifndef RAY_H
#define RAY_H

#include "Vector.h"

namespace MATH {

	// A Ray is defined using the equation of a line in 3D space
		// Ray needs a starting position(Vec3) and a direction(Vec3) 
	struct Ray{

		Vec3 startPosition, direction;

		inline Ray(Vec3& newStartPosition, Vec3& newDirection) {
			startPosition = newStartPosition;
			direction = newDirection;
		}

		inline Ray(float startX, float startY, float startZ, float direcX, float direcY, float direcZ) {
			startPosition.set(startX, startY, startZ);
			direction.set(direcX, direcY, direcZ);
		}

		Ray(const Ray& r) {
			startPosition = r.startPosition;
			direction = r.direction;
		}

		// Returns the current position of the Ray at the magnitude of the passed float
		inline Vec3 CurrentPosition(float t) {
			return startPosition + direction * t;
		}

	};
}

#endif

