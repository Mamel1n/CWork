#ifndef NETWORK_H
#define NETWORK_H

#include <map>
#include <string>
#include "crutils.h"
#include "exceptions.h"

/** 
* @file network.h
* @brief Определения классов для управления сетевым взаимодействием.
* @details Этот файл содержит определения классов для управления сетевыми подключениями и передачей данных.
* @date 23.11.2024
* @version 1.0
* @authorsa Мамелин Д. А.
*/

/** 
* @brief Класс для управления сетевым подключением и взаимодействием.
*/
class Network
{
public:
    /**
    * @brief Конструктор класса Network.
    * @param address Адрес сервера.
    * @param port Порт сервера.
    * @param database База данных пользователей.
    * @param log_path Путь к файлу логов.
    */
    Network(
        const std::string &address,
        uint16_t port,
        const std::map<std::string, std::string> &database,
        const std::string &log_path);

    /**
    * @brief Метод для получения адреса сервера.
    * @return Адрес сервера.
    */
    std::string &getAddress();

    /**
    * @brief Метод для получения порта сервера.
    * @return Порт сервера.
    */
    uint16_t &getPort();

    /**
    * @brief Метод для получения базы данных пользователей.
    * @return База данных пользователей.
    */
    std::map<std::string, std::string> &getDatabase();

    /**
    * @brief Метод для ожидания соединений.
    * @throw NetworkException Если не удалось создать, привязать или слушать сокет.
    */
    void wait();

    /**
    * @brief Метод для аутентификации пользователя.
    * @throw NetworkException Если не удалось прочитать логин, найти логин в базе данных, отправить или получить данные от клиента.
    */
    void auth();

    /**
    * @brief Метод для вычисления суммы значений в векторах.
    * @throw NetworkException Если не удалось прочитать данные от клиента или отправить результат.
    */
    void calc();

    /**
    * @brief Метод для закрытия клиентского соединения.
    */
    void close();

    /**
    * @brief Метод для закрытия основного соединения.
    */
    void quit();

private:
    std::string address;                         ///< Адрес сервера.
    uint16_t port;                               ///< Порт сервера.
    std::map<std::string, std::string> database; ///< База данных пользователей.
    int socket;                                  ///< Сокет сервера.
    int client_socket;                           ///< Сокет клиента.
    std::string log_path;                        ///< Путь к файлу логов.
};

#endif // NETWORK_H
