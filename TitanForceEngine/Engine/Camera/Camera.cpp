#include "Camera.h"

#include"../Core/Engine.h"
#include "../Devices/Window.h"


CameraComponent::CameraComponent() :
	Component( ID ),
	m_pWindow( nullptr )
{
	m_position = glm::vec3();
	m_fieldOfView = 45.0f;
	m_forward = glm::vec3( 0.0f, 0.0f, -1.0f );
	m_up = glm::vec3( 0.0f, 1.0f, 0.0f );
	m_worldUp = m_up;
	m_nearPlane = 1.0f;
	m_farPlane = 50.0f;
	m_yaw = -90.0f;
	m_pitch = 0.0f;

	m_pWindow = Engine::Get()->GetWindow();

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

}

CameraComponent::CameraComponent( Window* window ) :
	Component( ID ),
	m_pWindow( window )
{
	m_position = glm::vec3();
	m_fieldOfView = 45.0f;
	m_forward = glm::vec3( 0.0f, 0.0f, -1.0f );
	m_up = glm::vec3( 0.0f, 1.0f, 0.0f );
	m_worldUp = m_up;
	m_nearPlane = 1.0f;
	m_farPlane = 50.0f;
	m_yaw = -90.0f;
	m_pitch = 0.0f;

	if ( m_pWindow == nullptr )
	{
		m_pWindow = Engine::Get()->GetWindow();
	}

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

}

CameraComponent::~CameraComponent() {}


void CameraSystem::Update( float deltaTime )
{
	for ( auto& c : m_components )
	{
		CameraComponent* camera = std::get<CameraComponent*>( c );

		if ( camera )
		{

			UpdateCameraVector( camera );
			UpdateCameraPosition( camera );
			UpdateCameraRotation( camera );

		}


	}
}


void CameraSystem::UpdateCameraVector( CameraComponent* camera )
{

	camera->m_forward.x = cos( glm::radians( camera->m_yaw ) ) * cos( glm::radians( camera->m_pitch ) );
	camera->m_forward.y = sin( glm::radians( camera->m_pitch ) );
	camera->m_forward.z = sin( glm::radians( camera->m_yaw ) ) * cos( glm::radians( camera->m_pitch ) );

	camera->m_forward = glm::normalize( camera->m_forward );

	camera->m_right = glm::normalize( glm::cross( camera->m_forward, camera->m_worldUp ) );

	camera->m_up = glm::normalize( glm::cross( camera->m_right, camera->m_forward ) );

}

// TODO: Implement some sort of Proxy to receive whether or not to update camera component's position and rotation
void CameraSystem::UpdateCameraPosition( CameraComponent * camera )
{
	// camera->m_position = camera->Some_Position_Proxy;

}

// TODO: Implement some sort of Proxy to receive whether or not to update camera component's position and rotation
void CameraSystem::UpdateCameraRotation( CameraComponent * camera )
{
	// camera->m_yaw = camera->Some_Yaw_Proxy;
	// camera->m_pitch = camera->Some_Pitch_Proxy;
	UpdateCameraVector( camera );
}
