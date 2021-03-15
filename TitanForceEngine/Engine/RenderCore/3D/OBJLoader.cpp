#include "OBJLoader.h"

#include "../../EntityComponentSystem/EntityComponentSystem/ECS/include/Utility/Debug.h"

#include <fstream>
#include <sstream>

// Loads Obj from the passed obj file name, if the file does not exist or is unreadable, this function returns null
SubMesh * OBJLoader::LoadObj( const std::string & filePath )
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::vector<int> indices, normalIndices, textureIndices;
	std::vector<Vertex> meshVertices;
	std::vector<SubMesh> subMeshes;
	glm::vec3 fVector;

	std::string relativeFilePath = "./Resources/Models/" + filePath;

	std::ifstream in( relativeFilePath.c_str(), std::ios::in );
	if ( !in )
	{
		DEBUG_LOG( LOG::ERRORLOG, "Cannot open OBJ file: " + relativeFilePath );
		CONSOLE_LOG( LOG::ERRORLOG, "Cannot open OBJ file: " + relativeFilePath );
		return nullptr;
	}
	std::string line;

	bool firstCheck = true;
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
	// Post Processing
	for ( int i = 0; i < indices.size(); i++ )
	{
		Vertex vert;
		vert.position = vertices[indices[i]];
		vert.normal = normals[normalIndices[i]];
		vert.texCoords = textureCoords[textureIndices[i]];
		meshVertices.push_back( vert );

	}

	SubMesh* subMesh = new SubMesh();
	subMesh->vertexList = meshVertices;
	subMesh->meshIndices = indices;
	return subMesh;
}
