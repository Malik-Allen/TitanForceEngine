#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <vector>
#include "../Kobe/Math.h"
#include "../Rendering/3D/Mesh.h"

using namespace Kobe;

class ObjLoader
{
public:
	ObjLoader();
	~ObjLoader();

	void LoadModel( const std::string& objFilePath_, const std::string& mtrlFilePath );
	void LoadModel( const std::string& filePath_ );
	std::vector<Vertex> GetVerts();
	std::vector<int> GetIndices();
	std::vector<SubMesh> GetSubMeshes();

	void OnDestroy();
	glm::vec3 fVector;

private:
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> textureCoords;
	std::vector<int> indices, normalIndices, textureIndices;
	std::vector<Vertex> meshVertices;
	std::vector<SubMesh> subMeshes;

	void PostProcessing();
	void LoadMaterial( const std::string& matName_ );
	void LoadMaterialLibrary( const std::string& matFilePath_ );

	bool firstCheck;

};

#endif