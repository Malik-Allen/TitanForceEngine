#include "Camera.h"


Camera::Camera()
{
	setProjectionMatrix( Math::Perspective( 45.0f, (16.0f / 9.0f), 0.5f, 100.0f ) );
	setViewMatrix( Math::LookAt( Vector3( 0.0, 0.0, 10.0f ), Vector3( 0.0f, 0.0f, 0.0f ), Vector3( 0.0f, 1.0f, 0.0f ) ) );
}

Camera::~Camera() {}