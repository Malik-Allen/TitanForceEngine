//#include "ShaderHandler.h"
//
//std::unique_ptr<ShaderHandler> ShaderHandler::shaderInstance = nullptr;
//std::map<std::string, GLuint> ShaderHandler::programs = std::map<std::string, GLuint>();
//
//ShaderHandler::ShaderHandler() {
//}
//
//ShaderHandler::~ShaderHandler() {
//	if (shaderInstance == nullptr) {
//		OnDestroy();
//	}
//}
//
//ShaderHandler* ShaderHandler::GetInstance() {
//	if (shaderInstance.get() == nullptr) {
//		shaderInstance.reset(new ShaderHandler);
//	}
//	return shaderInstance.get();
//}
//
//void ShaderHandler::CreateProgram(const std::string& shaderName_, const std::string& vertexShaderFileName_, const std::string& fragmentShaderFileName_) {
//	std::vector<char> vertexShaderCode = ReadFile(vertexShaderFileName_);
//	std::vector<char> fragmentShaderCode = ReadFile(fragmentShaderFileName_);
//
//
//
//	if (vertexShaderCode == "" || fragmentShaderCode == "") {
//		return;
//	}
//
//	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderCode, shaderName_);
//	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode, shaderName_);
//
//	if (vertexShader == 0 || fragmentShader == 0) {
//		return;
//	}
//
//	GLint linkResult = 0;
//	GLuint program = glCreateProgram();
//	glAttachShader(program, vertexShader);
//	glAttachShader(program, fragmentShader);
//
//	glLinkProgram(program);
//
//	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
//
//	if (!linkResult) {
//		GLint infoLogLength = 0;
//		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
//		std::vector<char> programLog(infoLogLength);
//		glGetProgramInfoLog(program, infoLogLength, NULL, &programLog[0]);
//		std::string programString(programLog.begin(), programLog.end());
//		Debug::Error("Failed to link shader " + shaderName_ + ". Error: " + programString, "ShaderHandler.cpp", __LINE__);
//		glDeleteShader(vertexShader);
//		glDeleteShader(fragmentShader);
//		glDeleteProgram(program);
//		return;
//	}
//
//	programs[shaderName_] = program;
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//
//}
//
//GLuint ShaderHandler::GetShader(const std::string& shaderName_) {
//	if (programs.find(shaderName_) != programs.end()) {
//		return programs[shaderName_];
//	}
//	return 0;
//}
//
//void ShaderHandler::OnDestroy() {
//	if (programs.size() > 0) {
//		for (auto p : programs) {
//			glDeleteProgram(p.second);
//
//		}
//	}
//	programs.clear();
//
//}
//
//std::vector<char> ShaderHandler::ReadFile(const std::string& filePath) {
//	std::ifstream file(filePath, std::ios::ate | std::ios::binary);
//
//	if (!file.is_open()) {
//		throw std::runtime_error("Failed to open file!");
//	}
//
//	// An advantage of starting at the bottom of the file:
//		// We can determine the size of the file and create a buffer
//	size_t fileSize = (size_t)file.tellg();
//	std::vector<char> buffer(fileSize);
//
//	file.seekg(0);
//	file.read(buffer.data(), fileSize);
//
//	file.close();
//
//	return buffer;
//}
//
//VkShaderModule ShaderHandler::CreateShaderModuleAndInfo(const std::vector<char>& shaderCode) {
//	VkShaderModuleCreateInfo shaderModuleInfo = {};
//	shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//	shaderModuleInfo.codeSize = shaderCode.size();
//	shaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());
//
//	VkShaderModule newModule;
//
//	if (vkCreateShaderModule(device, &shaderModuleInfo, nullptr, &newModule) != VK_SUCCESS) {
//		throw std::runtime_error("Failed to create shader module!");
//	}
//
//	return newModule;
//}
//
//
//GLuint ShaderHandler::CreateShader(GLenum shaderType_, const std::string& source_, const std::string& shaderName_) {
//	GLint compileResult = 0;
//	GLuint shader = glCreateShader(shaderType_);
//	const char* shaderCodePtr = source_.c_str();
//	const int shaderCodeSize = source_.size();
//
//	glShaderSource(shader, 1, &shaderCodePtr, &shaderCodeSize);
//	glCompileShader(shader);
//
//	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
//
//	if (!compileResult) {
//		GLint infoLogLength = 0;
//		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
//		std::vector<char> shaderLog(infoLogLength);
//		glGetShaderInfoLog(shader, infoLogLength, NULL, &shaderLog[0]);
//		std::string shaderString(shaderLog.begin(), shaderLog.end());
//		Debug::Error("Error Compiling shader " + shaderName_ + ". Error: \n" + shaderString, "ShaderHandler.cpp", __LINE__);
//		return 0;
//	}
//
//	return shader;
//}
//#include "ShaderHandler.h"
