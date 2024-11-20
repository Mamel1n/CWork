#ifndef INTERFACE_H
#define INTERFACE_H

#include <map>
#include <string>
#include "exceptions.h"
#include "network.h"

/** 
* @file interface.h
* @brief Определение класса интерфейса.
* @details Этот файл содержит определения классов для обработки параметров командной строки, чтения базы данных паролей и запуска сервера.
* @date 23.11.2024
* @version 1.0
* @authorsa Мамелин Д. А.
*/

/** 
* @brief Класс для управления интерфейсом программы.
*/
class Interface
{
public:
    /**
    * @brief Конструктор принимает параметры командной строки.
    * @param argc Количество аргументов командной строки.
    * @param argv Аргументы командной строки.
    * @throw ArgsDecodeException Если аргументы командной строки некорректны.
    */
    Interface(int argc, char *argv[]);

    /**
    * @brief Метод для парсинга аргументов командной строки.
    * @param argc Количество аргументов командной строки.
    * @param argv Аргументы командной строки.
    * @throw ArgsDecodeException Если аргументы командной строки некорректны.
    */
    void parseArgs(int argc, char *argv[]);

    /**
    * @brief Метод для чтения базы данных паролей и записи в атрибут map.
    * @throw IOException Если не удалось открыть или прочитать файл базы данных.
    * @throw DataDecodeException Если строки в базе данных имеют некорректный формат или произошла ошибка при чтении файла.
    */
    void readDB();

    /**
    * @brief Метод для запуска сервера.
    * @throw NetworkException Если возникли ошибки при запуске сервера.
    */
    void run();

    /**
    * @brief Метод для вывода справки.
    */
    void showHelp() const;

    /**
    * @brief Метод для получения объекта Network.
    * @return Указатель на объект класса Network.
    */
    Network *getNetwork();

    /**
    * @brief Метод для получения пути к файлу базы данных.
    * @return Путь к файлу базы данных.
    */
    std::string &getDbPath();

    /**
    * @brief Метод для получения пути к файлу логов.
    * @return Путь к файлу логов.
    */
    std::string &getLogPath();

private:
    Network *network; ///< Указатель на объект класса Network.
    std::string db_path; ///< Путь к файлу базы данных.
    std::string log_path; ///< Путь к файлу логов.
    int port; ///< Порт.
    std::string address; ///< Адрес.
    std::map<std::string, std::string> database; ///< Словарь для хранения базы данных паролей.
};

#endif // INTERFACE_H
