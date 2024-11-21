/**
* @file main.cpp
* @brief Тесты для проверки функциональности различных компонентов.
* @details Этот файл содержит тесты для проверки парсинга аргументов командной строки, генерации соли, вычисления хеша, и работы сетевого взаимодействия.
* @date 23.11.2024
* @version 1.0
* @authorsa Мамелин Д. А.
*/

#include <UnitTest++/UnitTest++.h>
#include "../../server/source/modules/crutils.h"
#include "../../server/source/modules/network.h"
#include "../../server/source/modules/interface.h"
#include "../../server/source/modules/exceptions.h"
#include <iostream>
#include <fstream>
#include <map>

/**
 * @brief Тест для проверки парсинга аргументов командной строки.
 */
TEST(InterfaceInitTest)
{
    const char *argv[] = {
        "vserver",
        "-pdb", "./db.txt",
        "-pl", "./log.txt"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    Interface interface(argc, const_cast<char **>(argv));
    CHECK_EQUAL(std::string("./db.txt"), interface.getDbPath());
    CHECK_EQUAL(std::string("./log.txt"), interface.getLogPath());
}

/**
 * @brief Тест для проверки обработки исключений при неверных аргументах.
 */
TEST(InvalidArgsTest)
{
    const char *argv[] = {
        "vserver",
        "-pdb", "./db.txt",
        "-pl", "./log.txt",
        "--unknown", "value"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    CHECK_THROW(Interface interface(argc, const_cast<char **>(argv)), ArgsDecodeException);
}

/**
 * @brief Тест для проверки генерации соли.
 */
TEST(GenerateSaltTest)
{
    std::string salt = crutils::generateSalt();

    // Проверяем, что длина соли равна 16 символам
    CHECK_EQUAL(16, salt.length());

    // Проверяем, что соль состоит только из шестнадцатеричных символов (0-9, A-F)
    for (char c : salt)
    {
        CHECK(((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')));
    }
}

/**
 * @brief Тест для проверки вычисления хеша.
 */
TEST(CalculateHashTest)
{
    std::string data = "test_data";
    std::string hash = crutils::calculateHash(data);

    // Проверяем, что длина хеша равна 40 символам
    CHECK_EQUAL(40, hash.length());

    // Проверяем, что хеш состоит только из шестнадцатеричных символов (0-9, A-F)
    for (char c : hash)
    {
        CHECK(((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')));
    }
}

/**
 * @brief Дополнительный тест для проверки корректности вычисления хеша с известными данными.
 */
TEST(CalculateHashKnownDataTest)
{
    std::string data = "hello";
    std::string expected_hash = "AAF4C61DDCC5E8A2DABEDE0F3B482CD9AEA9434D";
    std::string hash = crutils::calculateHash(data);

    // Проверяем, что хеш совпадает с ожидаемым значением
    CHECK_EQUAL(expected_hash, hash);
}

/**
 * @brief Тест для проверки, что хеш не повторяется на разных данных.
 */
TEST(CalculateHashDifferentDataTest)
{
    std::string data1 = "data1";
    std::string data2 = "data2";

    std::string hash1 = crutils::calculateHash(data1);
    std::string hash2 = crutils::calculateHash(data2);

    // Проверяем, что хеши разных данных не совпадают
    CHECK(hash1 != hash2);
}

/**
 * @brief Тест для проверки, что хеш повторяется на одинаковых данных.
 */
TEST(CalculateHashSameDataTest)
{
    std::string data = "same_data";

    std::string hash1 = crutils::calculateHash(data);
    std::string hash2 = crutils::calculateHash(data);

    // Проверяем, что хеши одинаковых данных совпадают
    CHECK_EQUAL(hash1, hash2);
}

/**
 * @brief Тест для проверки, что соль генерируется разная.
 */
TEST(GenerateDifferentSaltTest)
{
    std::string salt1 = crutils::generateSalt();
    std::string salt2 = crutils::generateSalt();

    // Проверяем, что соли разные
    CHECK(salt1 != salt2);
}

/**
 * @brief Тест для проверки создания сокета и ожидания соединений.
 */
TEST(NetworkInitializationTest)
{
    std::map<std::string, std::string> database;
    database["user"] = "P@ssW0rd";
    Network network("127.0.0.1", 12345, database, "./log.txt");
    CHECK_EQUAL(std::string("127.0.0.1"), network.getAddress());
    CHECK_EQUAL((uint16_t)12345, network.getPort());
    CHECK_EQUAL(std::string("P@ssW0rd"), network.getDatabase().at("user"));
    network.quit();
}

/**
 * @brief Тест для проверки успешного взаимодейсвия.
 */
TEST(NetworkSuccessTest)
{
    std::map<std::string, std::string> database;
    database["user"] = "P@ssW0rd";
    Network network("127.0.0.1", 33333, database, "./log.txt");

    // Попробуем аутентифицировать пользователя
    network.wait();
    network.auth();
    network.calc();
    network.quit();
}

/**
 * @brief Класс логгера для вывода детализированной информации о тестах.
 */
class MyTestReporter : public UnitTest::TestReporter
{
public:
    /**
    * @brief Сообщает о начале выполнения теста.
    * @param test Детали теста.
    */
    void ReportTestStart(UnitTest::TestDetails const &test) override
    {
        std::cout << "Test <" << test.testName << "> started:\n";
    }

    /**
    * @brief Сообщает об окончании выполнения теста.
    * @param test Детали теста.
    * @param secondsElapsed Время выполнения теста в секундах.
    */
    void ReportTestFinish(UnitTest::TestDetails const &test, float secondsElapsed) override
    {
        std::cout << "*passed("
                  << secondsElapsed << " seconds)\n"
                  << "================================"
                  << "================================\n";
    }

    /**
    * @brief Сообщает о сбое теста.
    * @param test Детали теста.
    * @param failure Описание сбоя.
    */
    void ReportFailure(UnitTest::TestDetails const &test, char const *failure) override
    {
        std::cout << "*failed: "
                  << " (" << failure << ")\n"
                  << "================================"
                  << "================================\n";
    }

    /**
    * @brief Сообщает об общей сводке выполнения тестов.
    * @param totalTestCount Общее количество тестов.
    * @param failedTestCount Количество неудачных тестов.
    * @param failureCount Общее количество сбоев.
    * @param secondsElapsed Общее время выполнения тестов в секундах.
    */
    void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed) override
    {
        std::cout << "Summary: "
                  << totalTestCount << " tests, "
                  << failedTestCount << " failed, "
                  << failureCount << " failures, "
                  << secondsElapsed << " seconds\n";
    }
};

/**
 * @brief Главная функция тестирования.
 * @details Инициализирует объект MyTestReporter и запускает тесты с использованием UnitTest++.
 * @param argc Количество аргументов командной строки.
 * @param argv Аргументы командной строки.
 * @return Код завершения программы. 0 - успешное завершение, 1 - ошибка.
 */
int main(int argc, char *argv[])
{
    MyTestReporter reporter;
    UnitTest::TestRunner runner(reporter);
    return runner.RunTestsIf(UnitTest::Test::GetTestList(), nullptr, UnitTest::True(), 0);
}
