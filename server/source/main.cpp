/**
* @file main.cpp
* @brief Главный файл программы.
* @details Этот файл содержит функцию main, которая инициализирует интерфейс и запускает сервер.
* @date 23.11.2024
* @version 1.0
* @authorsa Мамелин Д. А.
*/

#include "modules/interface.h"
#include <iostream>

/**
 * @brief Главная функция программы.
 * @details Инициализирует объект Interface и запускает его. Обрабатывает все исключения, возникающие во время выполнения программы.
 * @param argc Количество аргументов командной строки.
 * @param argv Аргументы командной строки.
 * @return Код завершения программы. 0 - успешное завершение, 1 - ошибка.
 */
int main(int argc, char* argv[]) {
    try {
        Interface ui(argc, argv);
        ui.run();
    } catch (const Exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
