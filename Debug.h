#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

/// Debug Class functionalities include:
/// Message Error Types that provide more clarity as to the origin of the error
/// Can be called from any class as long as Debug.h is an include directory
/// Prints all errors to a .txt

enum MessageType : unsigned short {
	TYPE_NONE = 0,
	TYPE_FATAL_ERROR,
	TYPE_ERROR,
	TYPE_WARNING,
	TYPE_TRACE,
	TYPE_INFO
};

class Debug {

private:
	static MessageType m_currentSeverity;
	static std::string m_fileName;

	/*Used as the functionality behind all other debugs functions*/
	static void Log(const MessageType type,
		const std::string& message,
		const std::string& fileName,
		const int line);


public:
	/*Removing some functionaility that could lead to memory mismanagement*/
	Debug(const Debug&) = delete;
	Debug(Debug&&) = delete;
	Debug& operator=(const Debug&) = delete;
	Debug& operator=(Debug&&) = delete;

	Debug() = delete;

	static void DebugInit(const std::string& fileName);
	static void SetSeverity(MessageType type);
	

	static void Info(const std::string& message,
		const std::string& fileName,
		const int line);

	static void Trace(const std::string& message,
		const std::string& fileName,
		const int line);

	static void Warning(const std::string& mesage,
		const std::string& fileName,
		const int line);

	static void Error(const std::string& message,
		const std::string& fileName,
		const int line);

	static void FatalError(const std::string& message,
		const std::string& fileName,
		const int line);

};

#endif

