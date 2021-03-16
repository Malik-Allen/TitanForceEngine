#include "Model.h"

#include "../Material/MaterialLoader.h"

#if GRAPHICS_API == GRAPHICS_OPENGL
#include "../../Graphics/OpenGL/3D/OpenGLMesh.h"
#include "../../Graphics/OpenGL/Texture/OpenGLTexture2D.h"
#include <gtc/type_ptr.hpp>
#elif GRAPHICS_API == GRAPHICS_VULKAN
#include "../../Graphics/Vulkan/3D/VulkanMesh.h"
#endif

#include "../Camera/Camera.h"

Model::Model(
	const char * objFileName,
	const char * materialFileName,
	ShaderLinker * shaderLinker,
	const char * textureFileName,
	TransformComponent * transformComponent )
{

#if GRAPHICS_API == GRAPHICS_OPENGL

	m_mesh = new OpenGLMesh( objFileName );

	m_texture = new OpenGLTexture2D( textureFileName );

#elif GRAPHICS_API == GRAPHICS_VULKAN

	m_mesh = new VulkanMesh( objFileName );

#else

	m_mesh = nullptr;

#endif

	m_material = MaterialLoader::LoadMaterial( materialFileName );
	m_shaderLinker = shaderLinker;
	m_transform = transformComponent;

}

Model::~Model()
{}

bool Model::OnCreate()
{
	return false;
}

void Model::Render( CameraComponent * camera )
{
#if GRAPHICS_API == GRAPHICS_OPENGL

	glUseProgram( m_shaderLinker->GetShaderProgramId() );

	glm::mat4 transform = m_transform->GetTransform();

	glUniformMatrix4fv( m_shaderLinker->GetUniformId( EShaderType::Vertex, "viewMatrix" ), 1, GL_FALSE, glm::value_ptr( camera->GetView() ) );
	glUniformMatrix4fv( m_shaderLinker->GetUniformId( EShaderType::Vertex, "projectionMatrix" ), 1, GL_FALSE, glm::value_ptr( camera->GetPerspective() ) );
	glUniformMatrix4fv( m_shaderLinker->GetUniformId( EShaderType::Vertex, "modelMatrix" ), 1, GL_FALSE, glm::value_ptr( transform ) );
	
	glm::mat3 normals = glm::transpose( glm::inverse( transform ) );
	glUniformMatrix3fv( m_shaderLinker->GetUniformId( EShaderType::Vertex, "normalMatrix" ), 1, GL_FALSE, glm::value_ptr( normals ) );

	glUniform3fv( m_shaderLinker->GetUniformId( EShaderType::Vertex, "lightPos" ), 1, glm::value_ptr( glm::vec3( 0.0f, 0.0f, 10.0f ) ) );

	if ( m_material )
	{
		// Handle Material Uniforms
	}

	if ( m_texture )
	{
		m_texture->Bind();
	}

	m_mesh->Render();

	if ( m_texture )
	{
		m_texture->Unbind();
	}
	

#elif GRAPHICS_API == GRAPHICS_VULKAN

	m_mesh = new VulkanMesh( objFileName );

#else

	m_mesh = nullptr;

#endif

}

void Model::OnDestroy()
{
	if ( m_mesh )
	{
		delete m_mesh;
		m_mesh = nullptr;
	}
	if ( m_material )
	{
		delete m_material;
		m_material = nullptr;
	}
	if ( m_shaderLinker )
	{
		delete m_shaderLinker;
		m_shaderLinker = nullptr;
	}
	if ( m_texture )
	{
		delete m_texture;
		m_texture = nullptr;
	}

}
