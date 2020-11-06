#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>

class Window;

class Camera
{

public:
	explicit Camera( Window* window );
	~Camera();

	void SetPosition( glm::vec3 position );
	void SetRotation( float yaw, float pitch );

	glm::mat4 GetView() const;
	glm::mat4 GetPerspective() const { return m_perspective; }
	glm::mat4 GetOrthographic() const { return m_orthographic; }
	glm::vec3 GetCameraPosition() const { return m_position; }
	glm::vec2 GetClippingPlanes() const;

	void ProcessMouseMovement( glm::vec2 offset );
	void ProcessMouseZoom( int y );

private:

	void UpdateCameraVector();
	void OnDestroy();

	Window*				m_pWindow;

	glm::vec3			m_position;
	glm::mat4			m_perspective;
	glm::mat4			m_orthographic;
	float				m_fieldOfView;
	float				m_yaw;
	float				m_pitch;
	float				m_roll;
	float				m_nearPlane;
	float				m_farPlane;
	glm::vec3			m_forward;
	glm::vec3			m_up;
	glm::vec3			m_right;
	glm::vec3			m_worldUp;

};
#endif