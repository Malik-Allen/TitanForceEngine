#include "GameObject.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"

GameObject::GameObject(Mesh *mesh_, Shader *shader_, Texture *texture_): 
	mesh(nullptr), shader(nullptr), texture(nullptr), bodyDef(nullptr), rb(nullptr) {
	
	mesh = mesh_;
	shader = shader_;
	texture = texture_;

	bodyDef = new BodyDefinition();
	rb = new RigidBody(bodyDef);

	modelMatrixID = shader->getUniformID("modelMatrix");
	normalMatrixID = shader->getUniformID("normalMatrix");

}

GameObject::~GameObject() {}

void GameObject::Update(float deltaTime_) {
	rb->Update(deltaTime_);
	setModelMatrix((MMath::translate(rb->m_velocity)) * (MMath::rotate(rb->m_angle, Vec3(0.0, 1.0, 0.0)))); 
}

void GameObject::Render() const {
	Matrix3 normalMatrix = modelMatrix;
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, modelMatrix);
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, normalMatrix);
	if (texture) {
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	}

	mesh->Render();

	/// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

/*The order of Matrix Multiplication matters!*/