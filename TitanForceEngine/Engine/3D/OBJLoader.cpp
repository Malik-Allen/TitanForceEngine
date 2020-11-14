#include "OBJLoader.h"

#include "../Debug/Debug.h"

#include <fstream>
#include <sstream>

LoadOBJModel::LoadOBJModel()
{
	vertices.reserve( 200 );
	normals.reserve( 200 );
	textureCoords.reserve( 200 );
	indices.reserve( 200 );
	normalIndices.reserve( 200 );
	textureIndices.reserve( 200 );
	meshVertices.reserve( 200 );
	subMeshes.reserve( 10 );

}

LoadOBJModel::~LoadOBJModel()
{}


void LoadOBJModel::LoadModel( const std::string& filePath_ )
{
	subMeshes.clear();

	std::ifstream in( filePath_.c_str(), std::ios::in );
	if ( !in )
	{
		Debug::Error( "Cannot open OBJ file: " + filePath_, __FILE__, __LINE__ );
		return;
	}
	std::string line;

	firstCheck = true;

	while ( std::getline( in, line ) )
	{
//VERTEX DATA
		if ( line.substr( 0, 2 ) == "v " )
		{
			std::stringstream v( line.substr( 2 ) );
			float x, y, z;
			v >> x >> y >> z;
			if ( firstCheck )
			{
				fVector.x = x;
				fVector.y = y;
				fVector.z = z;
				firstCheck = false;
			}
			//check max for the radius
			if ( abs( x ) >= fVector.x )
			{
				fVector.x = abs( x );
			}
			if ( abs( y ) >= fVector.y )
			{
				fVector.y = abs( y );
			}
			if ( abs( z ) >= fVector.z )
			{
				fVector.z = abs( z );
			}

			vertices.push_back( glm::vec3( x, y, z ) );
		}
		//NORMAL DATA
		else if ( line.substr( 0, 3 ) == "vn " )
		{
			std::stringstream v( line.substr( 3 ) );
			float x, y, z;
			v >> x >> y >> z;
			normals.push_back( glm::vec3( x, y, z ) );
		}
		//TEXTURE DATA
		else if ( line.substr( 0, 3 ) == "vt " )
		{
			std::stringstream v( line.substr( 3 ) );
			glm::vec2 vec;
			v >> vec.x >> vec.y;
			textureCoords.push_back( vec );
		}
		//FACE DATA
		else if ( line.substr( 0, 2 ) == "f " )
		{
			std::string data = line.substr( 2 );
			int currentEndPos = 0;
			while ( ( currentEndPos = data.find( ' ' ) ) != std::string::npos )
			{
				std::string chunk = data.substr( 0, currentEndPos );
				int subEndPos = 0;
				int vector = 0;


				for ( int i = 0; i < 3; i++ )
				{
					int pushBack;
					if ( ( subEndPos = chunk.find( '/' ) ) != std::string::npos )
					{

						pushBack = std::stoi( chunk.substr( 0, subEndPos ) );

						chunk.erase( 0, subEndPos + 1 );
					}
					else
					{
						pushBack = std::stoi( chunk );
					}

					pushBack--;

					switch ( i )
					{
					default:
					case 0:
						indices.push_back( pushBack );
						break;
					case 1:
						textureIndices.push_back( pushBack );
						break;
					case 2:
						normalIndices.push_back( pushBack );
						break;
					}
				}

				vector = 0;
				data.erase( 0, currentEndPos + 1 );
			}

		}
	}
	PostProcessing();

}

std::vector<Vertex> LoadOBJModel::GetVerts()
{
	return meshVertices;
}

std::vector<int> LoadOBJModel::GetIndices()
{
	return indices;
}

std::vector<SubMesh> LoadOBJModel::GetSubMeshes()
{
	return subMeshes;
}

void LoadOBJModel::OnDestroy()
{
	vertices.clear();
	normals.clear();
	textureCoords.clear();
	indices.clear();
	normalIndices.clear();
	textureIndices.clear();
	meshVertices.clear();
	subMeshes.clear();
}

void LoadOBJModel::PostProcessing()
{
	for ( int i = 0; i < indices.size(); i++ )
	{
		Vertex vert;
		vert.position = vertices[indices[i]];
		vert.normal = normals[normalIndices[i]];
		vert.texCoords = textureCoords[textureIndices[i]];
		meshVertices.push_back( vert );

	}
	SubMesh subMesh;

	subMesh.vertexList = meshVertices;
	subMesh.meshIndices = indices;

	subMeshes.push_back( subMesh );

	indices.clear();
	normalIndices.clear();
	textureIndices.clear();
	meshVertices.clear();

}