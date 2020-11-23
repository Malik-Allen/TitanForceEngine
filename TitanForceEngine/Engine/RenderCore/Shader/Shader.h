#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <string>

enum class EShaderType
{
	Vertex,
	Tessellation,
	Geometry,
	Fragment,
	TOTAL
};

class Shader
{
	using UniformMap = std::map<std::string, unsigned int>;

	Shader( const Shader& ) = delete;
	Shader& operator=( const Shader& ) = delete;
	Shader( Shader&& ) = delete;
	Shader& operator = ( Shader&& ) = delete;

public:

	Shader(const EShaderType& shaderType, const std::string& fileName);
	~Shader();

	// Returns the location id for this shader
	unsigned int GetShaderId() const;

	// Returns the location id for the passed uniform name on this shader
	unsigned int GetUniformId( const std::string& uniformName );

	const std::string& GetFileName() const;

	const EShaderType& GetType() const;

	void SetUpUniformLocations( unsigned int programId );

private:

	EShaderType		m_type;
	unsigned int	m_id;
	UniformMap		m_uniformMap;
	std::string		m_fileName;
	std::string		m_shaderSource;

	std::string ReadShaderFromFile( const std::string& fileName );
	unsigned int CompileShader();
	

};

#endif // !SHADER_H

