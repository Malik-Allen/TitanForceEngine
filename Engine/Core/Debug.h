#ifndef DEBUG_H
#define DEBUHG_H

#include <iostream>
#include <string>
#include <fstream>

enum class MessageType : unsigned short {
	TYPE_NONE = 0,
	TYPE_FATAL_ERROR,
	TYPE_ERROR,
	TYPE_WARNING,
	TYPE_TRACE,
	TYPE_INFO
};

class Debug {

public:
	// Debug class will not be instatiated or copy so the copy, remove and init constructors are deleted
	Debug() = delete;
	Debug(const Debug&) = delete;
	Debug& operator=(const Debug&) = delete;
	Debug(Debug&&) = delete;
	Debug& operator=(Debug&&) = delete;
	

	static void DebugInit();
	static void SetDebugFileName();
	static void SetSeverity(MessageType type);

	// NOTE: Aditional functions will be needed for additional debug types
	static void FatalError(const std::string& message_, const std::string& fileName_, const int line_);
	static void Error(const std::string& message_, const std::string& fileName_, const int line_);
	static void Warning(const std::string& message_, const std::string& fileName_, const int line_);
	static void Trace(const std::string& message_, const std::string& fileName_, const int line_);
	static void Info(const std::string& message_, const std::string& fileName_, const int line_);



private:
	static void Log(const MessageType type_, const std::string& message_, const std::string& fileName_, const int line_);
	static MessageType currentSeverity;
	static std::string& fileName;

};

#endif