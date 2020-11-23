#ifndef SHADERLINKER_H
#define SHADERLINKER_H

#include "Shader.h"

#include <string>
#include <array>


class ShaderLinker
{
	ShaderLinker( const ShaderLinker& ) = delete;
	ShaderLinker& operator=( const ShaderLinker& ) = delete;
	ShaderLinker( ShaderLinker&& ) = delete;
	ShaderLinker& operator = ( ShaderLinker&& ) = delete;

public:

	static constexpr uint32_t m_uniqueShaderCount = static_cast<uint32_t>( EShaderType::TOTAL );

	ShaderLinker( const std::string& shaderName );
	~ShaderLinker();

	// Adds the passed shader to this linker, linking this shader to this program
	void SubmitShader( Shader* shader );

	// Return the uniform id for the passed uniform name from the shader inside of this linker with the passed shader type
	unsigned int GetUniformId( const EShaderType& shaderType, const std::string& uniformName ) const;

	unsigned int GetShaderProgramId() const;

	// Links/ Binds all shaders submitted to this shader linker
	void LinkShaders();

private:

	std::string				m_name;
	unsigned int			m_id;
	std::array<Shader*, m_uniqueShaderCount>	m_shaderChain;


};

#endif // !SHADERLINKER_H

