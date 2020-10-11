#ifndef SHADER_H
#define SHADER_H

#include <vulkan.h>
#include <vector>

namespace Vulkan {

	class Shader {

	public:

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader(Shader&&) = delete;
		Shader& operator = (Shader&&) = delete;

		Shader(const VkDevice& device_, VkShaderStageFlagBits type_, const std::string& shaderName, const std::string& fileName, const std::string& entryPoint_);
		~Shader();

		// Returns the Shader Stage Flags
		VkShaderStageFlagBits GetType() const { return type; }

		// Returns Stage infor for this shader
		VkPipelineShaderStageCreateInfo GetStageInfo() const { return shaderStageInfo; }
		
		void OnDestroy();	// Handles the destruction of this shader

	private:

		// Creates this shader's shadule module
		void OnCreate(const VkDevice& device_, VkShaderStageFlagBits type_, const std::string& shaderName, const std::string& fileName, const std::string& entryPoint_ = "main");

		VkDevice device;
		VkShaderStageFlagBits type;	

		const std::string name;	// name of this shaderS
		const std::string entryPoint;	// function entry point for shader

		VkShaderModule shaderModule;
		VkShaderModuleCreateInfo shaderModuleInfo;
		VkPipelineShaderStageCreateInfo shaderStageInfo;

		std::vector<char> ReadFile(const std::string& fileName);	// Reads in all the bytes from the specified file, returns them in a byte array, managed by std::vector
		VkShaderModule CreateShaderModule(const std::vector<char>& shaderCode);	// Create a ShaderModule, returns newly created shader module
		VkPipelineShaderStageCreateInfo CreateShaderStageInfo(VkShaderStageFlagBits type_, const std::string& entryPoint_);	//

	};


}


#endif // !SHADER_H
