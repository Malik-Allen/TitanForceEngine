//#ifndef SHADERHANDLER_H
//#define SHADERHANDLER_H
//
//#include <sstream>
//#include <map>
//#include <vector>
//#include <memory>
//#include <vulkan.h>
//#include "../../Debug/Debug.h"
//
//class ShaderHandler {
//public:
//	ShaderHandler(const ShaderHandler&) = delete;
//	ShaderHandler(ShaderHandler&&) = delete;
//	ShaderHandler& operator=(const ShaderHandler&) = delete;
//	ShaderHandler& operator=(ShaderHandler&&) = delete;
//
//	static ShaderHandler* GetInstance();
//
//	// Create a shader module using the passed vertex ad fragment shader file names
//	void CreateShaderModule(const std::string& shaderName, const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName);
//	
//	// Returns a shader module with the name
//	VkShaderModule GetShaderModule(const std::string& shaderName);
//
//	void OnDestroy();
//
//private:
//	ShaderHandler();
//	~ShaderHandler();
//
//	static std::unique_ptr<ShaderHandler> shaderInstance;
//	friend std::default_delete<ShaderHandler>;
//
//	// Used to read the shader file
//	std::vector<char> ReadFile(const std::string& filePath);
//
//	// Creates a shader module from the shader code passed
//	VkShaderModule CreateShaderModuleAndInfo(const std::vector<char>& shaderCode);
//
//	
//
//	GLuint CreateShader(GLenum shaderType_, const std::string& source_, const std::string& shaderName_);
//
//	static std::map<std::string, GLuint> programs;
//
//	class VulkanInstance* vulkanInstance;
//};
//
//#endif