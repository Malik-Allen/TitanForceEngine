#ifndef MESH_H
#define MESH_H

#include <glm.hpp>
#include <vector>
#include <array>

#include <vulkan.h>

#include "../../Vulkan/Commands/CommandBuffer.h"
#include "../../Vulkan/Buffers/UniformBuffer.h"

#include "../../Core/Camera.h"

#include "../../Kobe/Math.h"


struct Vertex {

	enum {	// Attribute Locations
		POSITION = 0,
		NORMAL = 1,
		TEXCOORRDS = 2,
		COLOUR = 3,
		TOTAL = 4
	};


	Vector3 position;
	Vector3 normal;
	Vector2 texCoords;
	Vector3 colour;
	
	// The Vertex Binding will descrive at which to load data from memory throughout the vertices
	// Specifing the number of bytes between data entries and whether to move to the next data entry after each vertex or instance
	
	// Returns the binding description for this vertex
	static VkVertexInputBindingDescription GetBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	// The Attribute Description decribes how to extract a vertex attribute chunk of veretx data originating from a binding description
	// The number of attributes expected for our current state is 4: position, normal, texCoords & colour: https://vulkan-tutorial.com/Vertex_buffers/Vertex_input_description
	
	// Returns the Vertex Attributes for this vertex
	static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributesDesctiption;
		attributesDesctiption.resize(TOTAL);

		attributesDesctiption[POSITION].binding = 0;
		attributesDesctiption[POSITION].location = POSITION;
		attributesDesctiption[POSITION].format = VK_FORMAT_R32G32B32_SFLOAT;	// Float, vec3, vec2, etc.. have different formats
		attributesDesctiption[POSITION].offset = offsetof(Vertex, position);

		attributesDesctiption[NORMAL].binding = 0;
		attributesDesctiption[NORMAL].location = NORMAL;
		attributesDesctiption[NORMAL].format = VK_FORMAT_R32G32B32_SFLOAT;	
		attributesDesctiption[NORMAL].offset = offsetof(Vertex, normal);

		attributesDesctiption[TEXCOORRDS].binding = 0;
		attributesDesctiption[TEXCOORRDS].location = TEXCOORRDS;
		attributesDesctiption[TEXCOORRDS].format = VK_FORMAT_R32G32_SFLOAT;	
		attributesDesctiption[TEXCOORRDS].offset = offsetof(Vertex, texCoords);
		
		attributesDesctiption[COLOUR].binding = 0;
		attributesDesctiption[COLOUR].location = COLOUR;
		attributesDesctiption[COLOUR].format = VK_FORMAT_R32G32B32_SFLOAT;	
		attributesDesctiption[COLOUR].offset = offsetof(Vertex, colour);

		return attributesDesctiption;
	}

};

struct MVPUniform {

	Matrix4 model;
	Matrix4 view;
	Matrix4 proj;

};

struct SubMesh {

	std::vector<Vertex> vertexList;
	std::vector<int> meshIndices;
	// class Material material;
};

class Mesh {

public:

	Mesh(SubMesh subMesh, Vulkan::CommandBuffer* commandBuffer);
	~Mesh();

	void Render(Camera* camera, const Matrix4& modelMatrix);


private:

	SubMesh	m_subMesh;
	MVPUniform	m_mvpUniform;

	Vulkan::CommandBuffer*			m_commandBuffer;


	Vulkan::Buffer*					m_vertexBuffer;
	Vulkan::Buffer*					m_indexBuffer;
	Vulkan::Buffer*					m_mvpUniformBuffer;

public:

	void GenerateBuffers();

};



#endif // !MESH_H