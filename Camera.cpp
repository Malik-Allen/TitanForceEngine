#include "Camera.h"
#include "MMath.h"

Camera::Camera() {
	createProjection(45.0f, (16.0f / 9.0f), 0.5f, 300.0f);
	createView(Vec3(-25.0, 15.0, 75.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
}

Camera::~Camera() {}

void Camera::createProjection(float fovy, float aspect, float near, float far) {
	projection = MMath::perspective(fovy, aspect, near, far);
}

void Camera::createView(Vec3 pos, Vec3 at, Vec3 up) {
	view = MMath::lookAt(pos, at, up);
}