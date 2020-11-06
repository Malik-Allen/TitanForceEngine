#include "Camera.h"

#include "../Devices/Window.h"

#include <gtc/matrix_transform.hpp>

Camera::Camera(Window* window) :
	m_pWindow(window)
{
	m_position = glm::vec3();
	m_fieldOfView = 45.0f;
	m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_worldUp = m_up;
	m_nearPlane = 2.0f;
	m_farPlane = 50.0f;
	m_yaw = -90.0f;
	m_pitch = 0.0f;

	float aspect = static_cast<float>( m_pWindow->GetWidth() ) / static_cast<float>( m_pWindow->GetHeight() );

	m_perspective = glm::perspective( 
		m_fieldOfView, 
		aspect, 
		m_nearPlane, 
		m_farPlane
	);
	
	m_orthographic = glm::ortho( 
		0.0f, 
		static_cast<float>( m_pWindow->GetWidth() ), 
		0.0f, 
		static_cast<float>( m_pWindow->GetHeight() ), 
		-1.0f, 
		-1.0f 
	);

	UpdateCameraVector();

}

Camera::~Camera(){
	OnDestroy();
}

void Camera::SetPosition(glm::vec3 position)
{
	m_position = position;
	UpdateCameraVector();
}

void Camera::SetRotation(float yaw, float pitch)
{
	m_yaw = yaw;
	m_pitch = pitch;
	UpdateCameraVector();
}



glm::mat4 Camera::GetView() const
{
	return glm::lookAt(
		m_position, 
		m_position + m_forward, 
		m_up
	);
}



glm::vec2 Camera::GetClippingPlanes() const
{
	return glm::vec2(m_nearPlane, m_farPlane);
}



void Camera::ProcessMouseMovement(glm::vec2 offset)
{
	offset *= 0.05f;

	m_yaw += offset.x;
	m_pitch += offset.y;

	if ( m_pitch > 89.0f )
	{
		m_pitch = 89.0f;
	}
	if ( m_pitch < -89.0f )
	{
		m_pitch = -89.0f;
	}

	if ( m_yaw < 0.0f )
	{
		m_yaw += 360.0f;
	}
	if ( m_yaw > 360.0f )
	{
		m_yaw -= 360.0f;
	}



	UpdateCameraVector();
}

void Camera::ProcessMouseZoom(int y)
{
	if (y < 0 || y > 0) {
		m_position += static_cast<float>(y)* (m_forward * 2.0f);
	}
	UpdateCameraVector();
}


void Camera::UpdateCameraVector()
{

	m_forward.x = cos( glm::radians( m_yaw ) ) * cos( glm::radians( m_pitch ) );
	m_forward.y = sin( glm::radians( m_pitch ) );
	m_forward.z = sin( glm::radians( m_yaw ) ) * cos( glm::radians( m_pitch ) );

	m_forward = glm::normalize( m_forward );

	m_right = glm::normalize( glm::cross( m_forward, m_worldUp ) );

	m_up = glm::normalize( glm::cross( m_right, m_forward ) );

}

void Camera::OnDestroy() 
{}
