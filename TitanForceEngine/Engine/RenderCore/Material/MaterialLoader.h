#ifndef MATERIALLOADER_H
#define MATERIALLOADER_H

#include "Material.h"

#include <string>

class MaterialLoader
{

	MaterialLoader() = delete;	// Static class, no constructor needed
	MaterialLoader( const MaterialLoader& ) = delete;
	MaterialLoader& operator=( const MaterialLoader& ) = delete;
	MaterialLoader( MaterialLoader&& ) = delete;
	MaterialLoader& operator=( MaterialLoader&& ) = delete;

public:

	// Loads Material from the passed material file name, if the file does not exist or is unreadable, this function returns null
	static Material* LoadMaterial( const std::string & materialFileName );

};

#endif // !MATERIALLOADER_H

