#ifndef CAMERA_H
#define CAMERA_H

#include "../../EntityComponentSystem/ECS/ECS.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Window;

class CameraComponent : public ECS::Component
{

	friend class CameraSystem;

public:

	static constexpr uint64_t ID = GENERATE_ID( "CameraComponent" );

	CameraComponent();
	CameraComponent( Window* window );
	~CameraComponent();

	glm::mat4 GetView() const { return glm::lookAt( m_position, m_position + m_forward, m_up ); }
	glm::mat4 GetPerspective() const { return m_perspective; }
	glm::mat4 GetOrthographic() const { return m_orthographic; }
	glm::vec3 GetCameraPosition() const { return m_position; }
	glm::vec2 GetClippingPlanes() const { return glm::vec2( m_nearPlane, m_farPlane ); }

private:

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


// Updates Camera Component's Position and Rotation
class CameraSystem : public ECS::System<CameraComponent>
{

public:
	static constexpr uint64_t ID = GENERATE_ID( "CameraSystem" );

	CameraSystem() : System(ID) {}

	~CameraSystem() {}


	virtual void Update( float deltaTime ) override final;

private:

	void UpdateCameraVector(CameraComponent* camera);

	// TODO: Implement some sort of Proxy to receive whether or not to update camera component's position and rotation
	void UpdateCameraPosition( CameraComponent* camera );
	void UpdateCameraRotation( CameraComponent* camera );

};





#endif