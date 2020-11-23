#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm.hpp>


struct Material
{
	const char*		name;
	float			shininess;
	float			transparency;
	glm::vec3		ambience;
	glm::vec3		diffuse;
	glm::vec3		specular;
};


#endif // !MATERIAL_H

