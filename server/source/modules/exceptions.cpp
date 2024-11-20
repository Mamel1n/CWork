#include "exceptions.h"
#include <fstream>
#include <iostream>
#include <ctime>

// Реализация конструктора Exception
Exception::Exception(
	const std::string &name,
	const std::string &message,
	const std::string &func,
	const std::string &log_path,
	bool critical)
	: name(name), message(message), func(func), log_path(log_path), critical(critical)
{
	logException();
}

const char *Exception::what() const noexcept
{
	// Формирование сообщения об ошибке
	std::string critical_str = critical ? "Critical" : "NoCritical";
	message = "[" + critical_str + "] " + name + " in " + func + "\nMessage: " + message + ".";
	return message.c_str();
}

void Exception::logException() const
{
	std::ofstream log_file(log_path, std::ios_base::app);
	if (!log_file.is_open())
	{
		std::cerr << "Failed to open log file for writing.\n";
		return;
	}

	// Получение текущего времени и формирование записи лога
	std::time_t now = std::time(nullptr);
	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
	std::string critical_str = critical ? "Critical" : "NoCritical";
	log_file << "[" << critical_str << "]\t[" << buffer << "]\t"
			 << name << " in " << func << ": " << message << ".\n";

	log_file.close();
}

// Реализация исключений
IOException::IOException(
	const std::string &message,
	const std::string &func,
	const std::string &log_path,
	bool critical)
	: Exception("IOException", message, func, log_path, critical) {}

ArgsDecodeException::ArgsDecodeException(
	const std::string &message,
	const std::string &func,
	const std::string &log_path,
	bool critical)
	: Exception("ArgsDecodeException", message, func, log_path, critical) {}

DataDecodeException::DataDecodeException(
	const std::string &message,
	const std::string &func,
	const std::string &log_path,
	bool critical)
	: Exception("DataDecodeException", message, func, log_path, critical) {}

AuthException::AuthException(
	const std::string &message,
	const std::string &func,
	const std::string &log_path,
	bool critical)
	: Exception("AuthException", message, func, log_path, critical) {}

NetworkException::NetworkException(
	const std::string &message,
	const std::string &func,
	const std::string &log_path,
	bool critical)
	: Exception("NetworkException", message, func, log_path, critical) {}
