#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "../../Core/Renderer.h"

#include <glad/glad.h>
#include <memory>

#include <map>
#include <string>

class OpenGLRenderer : public Renderer
{
public:

	OpenGLRenderer();
	~OpenGLRenderer();

	virtual void OnCreate(
		const char* applicationName,
		const char* engineName,
		int version,
		bool enableValidationLayers,
		Window* window ) override final;

	virtual void OnDestroy() override final;

	virtual void Render() override final;
	virtual void Wait() override final;

	virtual void AddMesh( Mesh* mesh ) override final;

private:

	void CreateShaderProgram(
		const std::string& shaderName,
		const std::string& vertexShaderFileName,
		const std::string& fragmentShaderFileName
	);

	GLuint GetShaderProgram( const std::string& shaderName );




	std::string ReadShader( const std::string& filePath );
	GLuint CreateShader( GLenum shaderType, const std::string& source, const std::string& shaderName );




	Window*		m_window;
	std::map<std::string, GLuint> m_shaderPrograms;


	void GetInstalledOpenGLInfo( int* major, int* minor );


};


#endif // !OPENGLRENDERER_H

