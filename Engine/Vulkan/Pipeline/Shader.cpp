#include "Shader.h"

#include <fstream>

namespace Vulkan {

	Shader::Shader(const VkDevice& device_, VkShaderStageFlagBits type_, const std::string& shaderName, const std::string& fileName, const std::string& entryPoint_ = "main") :
		device(device_),
		type(type_),
		name(shaderName),
		entryPoint(entryPoint_),
		shaderModule(VK_NULL_HANDLE)	
	{
		OnCreate(device, type, name, fileName, entryPoint);
	}

	Shader::~Shader() {}


	// Creates this shader's shadule module
	void Shader::OnCreate(const VkDevice& device_, VkShaderStageFlagBits type_, const std::string& shaderName, const std::string& fileName, const std::string& entryPoint_) {

		device = device_;
		if (device == VK_NULL_HANDLE) {
			throw std::runtime_error("Null device passed to shader!");
		}

		auto shaderCode = ReadFile(fileName);

		shaderModule = CreateShaderModule(shaderCode);
		shaderStageInfo = CreateShaderStageInfo(type_, entryPoint_);
	
	}


	// Handles the destruction of this shader
	void Shader::OnDestroy() {
		vkDestroyShaderModule(device, shaderModule, nullptr);
	}
	

	// Reads in all the bytes from the specified file, returns them in a byte array, managed by std::vector
	std::vector<char> Shader::ReadFile(const std::string& fileName) {

		std::ifstream file(fileName, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("Failed to open file!");
		}

		// An advantage of starting at the bottom of the file:
			// We can determine the size of the file and create a buffer
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;

	}


	// Create a ShaderModule, returns newly created shader module
	VkShaderModule Shader::CreateShaderModule(const std::vector<char>& shaderCode) {

		shaderModuleInfo = {};
		shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleInfo.codeSize = shaderCode.size();
		shaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

		VkShaderModule newModule;

		if (vkCreateShaderModule(device, &shaderModuleInfo, nullptr, &newModule) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create shader module!");
		}

		return newModule;

	}

	VkPipelineShaderStageCreateInfo Shader::CreateShaderStageInfo(VkShaderStageFlagBits type_, const std::string& entryPoint_) {
		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = type_;
		shaderStageInfo.module = shaderModule;
		shaderStageInfo.pName = entryPoint_.c_str();
		return shaderStageInfo;
	}

}