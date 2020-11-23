#include "OpenGLMesh.h"


#include <gtc/type_ptr.hpp>

OpenGLMesh::OpenGLMesh( const char* objFileName ) :
	IMesh( objFileName ),
	VAO( 0 ), 
	VBO( 0 )
{
	GenerateBuffers();
}

OpenGLMesh::~OpenGLMesh()
{
	glDeleteVertexArrays( 1, &VAO );
	glDeleteBuffers( 1, &VBO );

}

void OpenGLMesh::GenerateBuffers()
{

	glGenVertexArrays( 1, &VAO );
	glGenBuffers( 1, &VBO );
	glBindVertexArray( VAO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, m_subMesh->vertexList.size() * sizeof( Vertex ), &m_subMesh->vertexList[0], GL_STATIC_DRAW );

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


}

void OpenGLMesh::Render()
{

	glBindVertexArray( VAO );
	glDrawArrays( GL_TRIANGLES, 0, m_subMesh->vertexList.size() );
	glBindVertexArray( 0 );

}
