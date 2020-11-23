#include "Debug.h"

 MessageType Debug::currentSeverity = MessageType::TYPE_NONE;

 void Debug::DebugInit(){
	 std::ofstream outputFile;
	 // We create the file using out just to make sure it is there and clears it
	 outputFile.open("TitanForceEngineLog.txt", std::ios::out);	
	 outputFile.close();

	 currentSeverity = MessageType::TYPE_FATAL_ERROR;
 }

 void Debug::SetSeverity(MessageType messgeType){
	 currentSeverity = messgeType;
 }

 void Debug::FatalError(const std::string& message_, const std::string& fileName_, const int line_) {
	 Log(MessageType::TYPE_FATAL_ERROR, "FATAL_ERROR: " + message_, fileName_, line_);
 }

 void Debug::Error(const std::string& message_, const std::string& fileName_, const int line_){
	 Log(MessageType::TYPE_ERROR, "ERROR: " + message_, fileName_, line_);
 }

 void Debug::Warning(const std::string& message_, const std::string& fileName_, const int line_) {
	 Log(MessageType::TYPE_WARNING, "WARNING: " + message_, fileName_, line_);
 }

 void Debug::Trace(const std::string& message_, const std::string& fileName_, const int line_) {
	 Log(MessageType::TYPE_TRACE, "TRACE: " + message_, fileName_, line_);
 }

 void Debug::Info(const std::string& message_, const std::string& fileName_, const int line_) {
	 Log(MessageType::TYPE_INFO, "INFO: " + message_, fileName_, line_);
 }

 void Debug::Log(const MessageType type_, const std::string& message_, const std::string& fileName_, const int line_) {
	 std::ofstream outputFile;
	 outputFile.open("TitanForceEngineLog.txt", std::ios::app | std::ios::out);

	 if (type_ <= currentSeverity && currentSeverity > MessageType::TYPE_NONE) {
		 outputFile << message_ << " in: " << fileName_ << " on line: " << line_ << std::endl;
	 }
	 outputFile.flush();
	 outputFile.close();

 }