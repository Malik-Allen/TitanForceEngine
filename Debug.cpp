#include "Debug.h"
// Absolute default values for this static class
MessageType Debug::m_currentSeverity = MessageType::TYPE_NONE;
std::string Debug::m_fileName = "";

// Creation of a Debug.txt given the fileName
void Debug::DebugInit(const std::string& fileName)
{
	m_fileName = fileName + ".txt";
	std::ofstream out;
	out.open(fileName, std::ios::out);
	out << "";
	out.close();

	m_currentSeverity = MessageType::TYPE_FATAL_ERROR;

}

//Set the severity of this debug
void Debug::SetSeverity(MessageType type) {
	m_currentSeverity = type;
}

void Debug::Log(const MessageType type, const std::string& message, const std::string& fileName, const int line) {

	if (type <= m_currentSeverity && m_currentSeverity > MessageType::TYPE_NONE) {

		std::ofstream out;
		out.open(fileName, std::ios::out | std::ios::app);
		out << message << " in file: " << fileName
			<< " on line: " << line << std::endl;
		out.flush();
		out.close();

	}
}

void Debug::Info(const std::string& message, const std::string& fileName, const int line) {
	Log(MessageType::TYPE_INFO, "[INFO]: " + message, fileName, line);
}
void Debug::Trace(const std::string& message, const std::string& fileName, const int line) {
	Log(MessageType::TYPE_TRACE, "[TRACE]: " + message, fileName, line);
}
void Debug::Warning(const std::string& message, const std::string& fileName, const int line) {
	Log(MessageType::TYPE_WARNING, "[WARNING]: " + message, fileName, line);
}
void Debug::Error(const std::string& message, const std::string& fileName, const int line) {
	Log(MessageType::TYPE_ERROR, "[ERROR]: " + message, fileName, line);
}
void Debug::FatalError(const std::string& message, const std::string& fileName, const int line) {
	Log(MessageType::TYPE_FATAL_ERROR, "[FATAL ERROR]: " + message, fileName, line);
}

