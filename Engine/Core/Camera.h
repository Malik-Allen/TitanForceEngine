#ifndef CAMERA_H
#define CAMERA_H

#include "../Kobe/Math.h"

using namespace Kobe;

class Camera
{
private:

	Matrix4 projection;
	Matrix4 view;

public:
	inline Matrix4 getProjectionMatrix() const { return projection; }
	inline Matrix4 getViewMatrix() const { return view; }
	inline void setProjectionMatrix( Matrix4 projection_ ) { projection = projection_; }
	inline void setViewMatrix( Matrix4 view_ ) { view = view_; }
	Camera();
	~Camera();
};

#endif