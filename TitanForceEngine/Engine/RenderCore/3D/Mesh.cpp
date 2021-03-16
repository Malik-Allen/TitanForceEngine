#include "Mesh.h"

#include "MeshLoader.h"

IMesh::IMesh( const char * objFileName ) :
	m_subMesh( MeshLoader::LoadMesh( objFileName ) )
{
	// ObjLoader will statically load in the passed obj file name
	// Storing the result inside of m_subMesh
}

IMesh::~IMesh()
{
	if ( m_subMesh )
	{
		if ( m_subMesh->vertexList.size() > 0 )
		{
			m_subMesh->vertexList.clear();
		}

		if ( m_subMesh->meshIndices.size() > 0 )
		{
			m_subMesh->meshIndices.clear();
		}

		delete m_subMesh;
		m_subMesh = nullptr;
	}

}
