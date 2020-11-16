#include "OpenGLMesh.h"

#include "../../../Camera/Camera.h"
#include "../../../FX/LightSource.h"

#include <gtc/type_ptr.hpp>

OpenGLMesh::OpenGLMesh( CameraComponent* camera, SubMesh subMesh, const char * shader ) :
	Mesh( camera, subMesh, shader ),
	VAO( 0 ), VBO( 0 ), shaderProgram( 0 ),
	modelLoc( 0 ), viewLoc( 0 ), projLoc( 0 ), normalLoc( 0 ),
	viewPositionLoc( 0 ), lightPositionLoc( 0 ), ambientLoc( 0 ), diffuseLoc( 0 ), colorLoc( 0 )
{}

OpenGLMesh::~OpenGLMesh()
{
	glDeleteVertexArrays( 1, &VAO );
	glDeleteBuffers( 1, &VBO );

	if ( m_subMesh.vertexList.size() > 0 )
	{
		m_subMesh.vertexList.clear();
	}

	if ( m_subMesh.meshIndices.size() > 0 )
	{
		m_subMesh.meshIndices.clear();
	}
}

void OpenGLMesh::GenerateBuffers()
{

	glGenVertexArrays( 1, &VAO );
	glGenBuffers( 1, &VBO );
	glBindVertexArray( VAO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, m_subMesh.vertexList.size() * sizeof( Vertex ), &m_subMesh.vertexList[0], GL_STATIC_DRAW );

	//Position
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)0 );

	//Normal
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)offsetof( Vertex, normal ) );

	//Texture Coordinates
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)offsetof( Vertex, texCoords ) );

	//Colour
	glEnableVertexAttribArray( 3 );
	glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)offsetof( Vertex, colour ) );

	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	modelLoc = glGetUniformLocation( shaderProgram, "modelMatrix" );
	viewLoc = glGetUniformLocation( shaderProgram, "viewMatrix" );
	projLoc = glGetUniformLocation( shaderProgram, "projectionMatrix" );

	normalLoc = glGetUniformLocation( shaderProgram, "normalMatrix" );

	// viewPositionLoc = glGetUniformLocation( shaderProgram, "viewPosition" );
	lightPositionLoc = glGetUniformLocation( shaderProgram, "lightPos" );
	// ambientLoc = glGetUniformLocation( shaderProgram, "lightSource.ambient" );
	// diffuseLoc = glGetUniformLocation( shaderProgram, "lightSource.diffuse" );
	// colorLoc = glGetUniformLocation( shaderProgram, "lightSource.lightColor" );

}

void OpenGLMesh::Render( glm::mat4 transform )
{

	// glUseProgram( shaderProgram );
	// glUniform3fv( viewPositionLoc, 1, glm::value_ptr( m_camera->GetCameraPosition() ) );
	glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( m_camera->GetView() ) );
	glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( m_camera->GetPerspective() ) );
	glUniform3fv( lightPositionLoc, 1, glm::value_ptr( glm::vec3( 0.0f, 0.0f, 10.0f ) ) );
	// glUniform1fv( ambientLoc, 1, m_camera->GetLightSources().front()->GetAmbient() );
	// glUniform1fv( diffuseLoc, 1, m_camera->GetLightSources().front()->GetDiffuse() );
	// glUniform3fv( colorLoc, 1, glm::value_ptr( m_camera->GetLightSources().front()->GetColour() ) );


	glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( transform ) );

	glm::mat3 normals = glm::transpose( glm::inverse( transform ) );

	glUniformMatrix3fv( normalLoc, 1, GL_FALSE, glm::value_ptr( normals ) );

	glBindVertexArray( VAO );
	glDrawArrays( GL_TRIANGLES, 0, m_subMesh.vertexList.size() );
	glBindVertexArray( 0 );

}
