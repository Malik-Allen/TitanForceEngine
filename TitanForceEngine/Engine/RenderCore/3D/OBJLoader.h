#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "Mesh.h"

#include <vector>
#include <string>

class OBJLoader
{


	OBJLoader() = delete;	// Static class, no constructor needed
	OBJLoader( const OBJLoader& ) = delete;
	OBJLoader& operator=( const OBJLoader& ) = delete;
	OBJLoader( OBJLoader&& ) = delete;
	OBJLoader& operator=( OBJLoader&& ) = delete;

public:

	// Loads Obj from the passed obj file name, if the file does not exist or is unreadable, this function returns null
	static SubMesh* LoadObj( const std::string& objFileName );

};


#endif // ! OBJLOADER_H

