#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "Mesh.h"

#include "../../EntityComponentSystem/ECS/ECS.h"

#include <sstream>


class OBJLoaderSystem : ECS::System<MeshComponent>
{

public:

	static constexpr uint64_t ID = GENERATE_ID( "OBJLoaderSystem" );

	OBJLoaderSystem();
	~OBJLoaderSystem();

	void LoadModel( const std::string& objFilePath_, const std::string& mtlFilePath_ );
	void LoadModel( const std::string& filePath_ );
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
	void LoadMaterial( const std::string& matName_ );
	void LoadMaterialLibrary( const std::string& matFilePath_ );
	bool firstCheck;
};

#endif // ! OBJLOADER_H

