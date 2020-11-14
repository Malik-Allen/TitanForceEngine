#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "Mesh.h"

#include <string>



class LoadOBJModel
{
public:

	LoadOBJModel();
	~LoadOBJModel();

	void LoadModel( const std::string& filePath );
	std::vector<Vertex> GetVerts();
	std::vector<int> GetIndices();
	std::vector<SubMesh> GetSubMeshes();

	void OnDestroy();
	glm::vec3 fVector;

private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::vector<int> indices, normalIndices, textureIndices;
	std::vector<Vertex> meshVertices;
	std::vector<SubMesh> subMeshes;

	void PostProcessing();

	bool firstCheck;
};


#endif // ! OBJLOADER_H

