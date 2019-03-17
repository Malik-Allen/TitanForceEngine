#ifndef SHADER_H
#define SHADER_H
#include "glew.h"
#include <unordered_map>

class Shader {
public:
	Shader(const Shader&) = delete;
	Shader(Shader&&) = delete;
	Shader& operator = (const Shader&) = delete;
	Shader& operator = (Shader&&) = delete;

	Shader(const char *vsFilename, const char *fsFilename);
	~Shader();

	inline GLuint getProgram() const { return shaderID;}
	inline GLuint getUniformID(std::string name) { return uniformMap[name]; }

private:	
	std::unordered_map<std::string, unsigned int> uniformMap;
	char* readTextFile(const char *fileName);	
	void readCompileAttach(const char *vsFilename, const char *fsFilename);
	void link();

	void setUniformLocations();

	unsigned int shaderID;
	unsigned int vertShaderID;
	unsigned int fragShaderID;
};
#endif