#ifndef AAB_H
#define AAB_H

#include "Plane.h"
#include "Vector.h"

namespace MATH {

struct AAB{

	float rx, ry, rz;
	Plane box[6];

	AAB() {
		rx = ry = rz = 1.0f;
		*this = AAB(rx, ry, rz);
	}

	AAB(float width, float height, float depth) {
		rx = width;
		ry = height;
		rz = depth;

		box[0] = Plane(1.0f, 0.0f, 0.0f, 0.0f);
		box[1] = Plane(0.0f, 1.0f, 0.0f, 0.0f);
		box[2] = Plane(0.0f, 0.0f, 1.0f, 0.0f);
		box[3] = Plane(1.0f, 0.0f, 0.0f, (-1.0f * rx));
		box[4] = Plane(0.0f, 1.0f, 0.0f, (-1.0f * ry));
		box[5] = Plane(0.0f, 0.0f, 1.0f, (-1.0f * rz));
		}
		
};
}

#endif