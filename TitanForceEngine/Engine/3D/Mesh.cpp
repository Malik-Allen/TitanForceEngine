#include "Mesh.h"

MeshComponent::MeshComponent() :
	Component( ID ),
	m_subMesh( SubMesh() ),
	m_uboMVP( UBO_MVP() )
{}

MeshComponent::MeshComponent( SubMesh subMesh ) :
	Component( ID ),
	m_subMesh( subMesh ),
	m_uboMVP( UBO_MVP() )
{}

MeshComponent::~MeshComponent()
{}
