#include "MeshLoader.h"

#include "../../EntityComponentSystem/EntityComponentSystem/ECS/include/Utility/Debug.h"

#include <fstream>
#include <sstream>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

// Loads Obj from the passed obj file name, if the file does not exist or is unreadable, this function returns null
SubMesh * MeshLoader::LoadMesh( const std::string & fileName )
{

	std::string relativeFilePath = "./Resources/Models/" + fileName;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;
	DEBUG_LOG( LOG::INFO, "Loading OBJ file: " + relativeFilePath );
	CONSOLE_LOG( LOG::INFO, "Loading OBJ file: " + relativeFilePath );

	if ( !tinyobj::LoadObj( &attrib, &shapes, &materials, &warn, &err, relativeFilePath.c_str() ) )
	{
		DEBUG_LOG( LOG::ERRORLOG, "Cannot open OBJ file: " + relativeFilePath );
		CONSOLE_LOG( LOG::ERRORLOG, "Cannot open OBJ file: " + relativeFilePath );
		CONSOLE_LOG( LOG::ERRORLOG, warn + err );
		throw std::runtime_error( warn + err );
	}

	SubMesh* subMesh = new SubMesh();

	for ( const auto& shape : shapes )
	{
		for ( auto index : shape.mesh.indices )
		{
			Vertex v;
			v.position.x = attrib.vertices[3 * index.vertex_index + 0];
			v.position.y = attrib.vertices[3 * index.vertex_index + 1];
			v.position.z = attrib.vertices[3 * index.vertex_index + 2];

			v.normal.x = attrib.normals[3 * index.normal_index + 0];
			v.normal.y = attrib.normals[3 * index.normal_index + 1];
			v.normal.z = attrib.normals[3 * index.normal_index + 2];

			v.texCoords.x = attrib.texcoords[2 * index.texcoord_index + 0];
			v.texCoords.y = attrib.texcoords[2 * index.texcoord_index + 0];

			v.colour = glm::vec3( 1.0f );

			subMesh->vertexList.push_back( v );
		}
	}

	return subMesh;


}

//Texture2D * Loader::LoadTexture2D( const std::string & fileName )
//{
//
//
//
//
//	return nullptr;
//}
