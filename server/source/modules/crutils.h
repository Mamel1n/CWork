#ifndef CRUTILS_H
#define CRUTILS_H

#include <string>

/** 
* @file crutils.h
* @brief Определения вспомогательных функций для криптографических операций.
* @details Этот файл содержит определения функций для генерации соли и вычисления хеша.
* @date 23.11.2024
* @version 1.0
* @authorsa Мамелин Д. А.
*/

/** 
* @brief Пространство имен для криптографических утилит.
*/
namespace crutils {
    /**
    * @brief Функция для генерации соли.
    * @return Соль в виде строки.
    */
    std::string generateSalt();

    /**
    * @brief Функция для вычисления хеша.
    * @param data Данные для хеширования.
    * @return Хеш в виде строки.
    */
    std::string calculateHash(const std::string &data);
}

#endif // CRUTILS_H
