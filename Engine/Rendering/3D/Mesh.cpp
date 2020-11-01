#include "Mesh.h"

#include "../../Vulkan/VulkanRenderer.h"

Mesh::Mesh( SubMesh subMesh, Vulkan::CommandBuffer* commandBuffer ) :
	m_subMesh(subMesh),
	m_commandBuffer(commandBuffer),
	m_vertexBuffer(nullptr),
	m_indexBuffer(nullptr)
{}

Mesh::~Mesh() {}

void Mesh::Render(Camera* camera, const Matrix4& modelMatrix )
{

	uint64_t mvpUniformSize = sizeof( MVPUniform );


	m_mvpUniform.proj = camera->getProjectionMatrix();
	m_mvpUniform.view = camera->getViewMatrix();
	m_mvpUniform.model = modelMatrix;

	// m_mvpUniformBuffer->UpdateData( mvpUniformSize, &m_mvpUniform );
	// m_commandBuffer->Begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );

	m_commandBuffer->Draw( m_subMesh.vertexList.size() );
	m_commandBuffer->DrawIndex( m_subMesh.meshIndices.size() );

	// m_commandBuffer->End();

}



void Mesh::GenerateBuffers() {
	
	// Creating the vertex buffer

	uint64_t verticesSize = sizeof(Vertex) * m_subMesh.vertexList.size();

	m_vertexBuffer = new Vulkan::Buffer(verticesSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_subMesh.vertexList.data());
	// m_vertexBuffer->OnCreate( verticesSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_subMesh.vertexList.data() );

	// Creating the index buffer
	uint64_t indicesSize = sizeof(int) * m_subMesh.meshIndices.size();

	m_indexBuffer = new Vulkan::Buffer(indicesSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_subMesh.meshIndices.data());

	// Binding vertex and index buffers
	std::vector<Vulkan::Buffer*> vertexBuffers;

	vertexBuffers.push_back( m_vertexBuffer );


	// Creating Uniform Buffer
	uint64_t mvpUniformSize = sizeof(MVPUniform);
	m_mvpUniformBuffer = new Vulkan::Buffer(mvpUniformSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &m_mvpUniform);

	
	Vulkan::VulkanRenderer::GetInstance()->BindVertexBuffers( vertexBuffers );

	Vulkan::VulkanRenderer::GetInstance()->BindIndexBuffer( m_indexBuffer );

}