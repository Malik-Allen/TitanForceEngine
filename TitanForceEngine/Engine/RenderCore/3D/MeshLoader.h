#ifndef LOADER_H
#define LOADER_H

#include "Mesh.h"

#include <vector>
#include <string>

class MeshLoader
{


	MeshLoader() = delete;	// Static class, no constructor needed
	MeshLoader( const MeshLoader& ) = delete;
	MeshLoader& operator=( const MeshLoader& ) = delete;
	MeshLoader( MeshLoader&& ) = delete;
	MeshLoader& operator=( MeshLoader&& ) = delete;

public:

	// Loads  from the passed  file name, if the file does not exist or is unreadable, this function returns null
	static SubMesh* LoadMesh( const std::string& fileName );

	/* 
		Loads Texture2D reference: return nullptr if loading fails
		@param fileName: Texture File Name
		@return: Reference to the loaded texture file
	*/
	// static Texture2D* LoadTexture2D( const std::string& fileName );

};


#endif // ! LOADER_H

