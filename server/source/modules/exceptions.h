#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

/** 
* @file exceptions.h
* @brief Определение классов исключений.
* @details Этот файл содержит определения классов исключений для обработки различных ошибок в программе.
* @date 23.11.2024
* @version 1.0
* @authorsa Мамелин Д. А.
*/

/**
* @brief Базовый класс для исключений.
*/
class Exception : public std::exception {
public:
    /**
    * @brief Конструктор класса Exception.
    * @param name Имя исключения.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникло исключение.
    * @param log_path Путь к файлу журнала.
    * @param critical Флаг критичности исключения.
    */
    Exception(const std::string &name,
              const std::string &message,
              const std::string &func,
              const std::string &log_path,
              bool critical = false);
    
    /**
    * @brief Метод для получения сообщения об ошибке.
    * @return Сообщение об ошибке.
    */
    const char* what() const noexcept override;

    /**
    * @brief Метод для логирования исключения.
    */
    void logException() const;

private:
    std::string name; ///< Имя исключения.
    std::string func; ///< Имя функции, в которой возникло исключение.
    std::string log_path; ///< Путь к файлу журнала.
    bool critical; ///< Флаг критичности исключения.
    mutable std::string message; ///< Сообщение об ошибке.
};

/**
* @brief Класс для исключений ввода-вывода.
*/
class IOException : public Exception {
public:
    /**
    * @brief Конструктор класса IOException.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникло исключение.
    * @param log_path Путь к файлу журнала.
    * @param critical Флаг критичности исключения.
    */
    IOException(const std::string &message,
                const std::string &func,
                const std::string &log_path,
                bool critical = false);
};

/**
* @brief Класс для исключений при декодировании аргументов.
*/
class ArgsDecodeException : public Exception {
public:
    /**
    * @brief Конструктор класса ArgsDecodeException.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникло исключение.
    * @param log_path Путь к файлу журнала.
    * @param critical Флаг критичности исключения.
    */
    ArgsDecodeException(const std::string &message,
                        const std::string &func,
                        const std::string &log_path,
                        bool critical = false);
};

/**
* @brief Класс для исключений при декодировании данных.
*/
class DataDecodeException : public Exception {
public:
    /**
   * @brief Конструктор класса DataDecodeException.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникло исключение.
    * @param log_path Путь к файлу журнала.
    * @param critical Флаг критичности исключения.
    */
    DataDecodeException(const std::string &message,
                        const std::string &func,
                        const std::string &log_path,
                        bool critical = false);
};

/**
* @brief Класс для исключений аутентификации.
*/
class AuthException : public Exception {
public:
    /**
    * @brief Конструктор класса AuthException.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникло исключение.
    * @param log_path Путь к файлу журнала.
    * @param critical Флаг критичности исключения.
    */
    AuthException(const std::string &message,
                  const std::string &func,
                  const std::string &log_path,
                  bool critical = false);
};

/**
* @brief Класс для сетевых исключений.
*/
class NetworkException : public Exception {
public:
    /**
    * @brief Конструктор класса NetworkException.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникло исключение.
    * @param log_path Путь к файлу журнала.
    * @param critical Флаг критичности исключения.
    */
    NetworkException(const std::string &message,
                     const std::string &func,
                     const std::string &log_path,
                     bool critical = false);
};

#endif // EXCEPTIONS_H
