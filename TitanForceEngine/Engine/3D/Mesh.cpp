#include "Mesh.h"

Mesh::Mesh( SubMesh subMesh ) :
	m_subMesh(subMesh),
	m_uboMVP( UBO_MVP())
{}

Mesh::~Mesh()
{}
