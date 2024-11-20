#include "interface.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>

// Конструктор принимает параметры командной строки
Interface::Interface(int argc, char *argv[])
{
    this->parseArgs(argc, argv);
    this->readDB();
    this->network = new Network(
        this->address,
        this->port,
        this->database,
        this->log_path);
}

Network *Interface::getNetwork(){
    return this->network;
};

std::string &Interface::getDbPath(){
    return this->db_path;
};

std::string &Interface::getLogPath(){
    return this->log_path;
};

// Метод для парсинга аргументов
void Interface::parseArgs(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            this->showHelp();
            exit(0);
        }
        else if ((strcmp(argv[i], "-pdb") == 0 || strcmp(argv[i], "--path_to_database") == 0) && (i + 1 < argc))
        {
            this->db_path = argv[i + 1];
            ++i; // Пропустить значение параметра
        }
        else if ((strcmp(argv[i], "-pl") == 0 || strcmp(argv[i], "--path_to_log") == 0) && (i + 1 < argc))
        {
            this->log_path = argv[i + 1];
            ++i; // Пропустить значение параметра
        }
        else if ((strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0) && (i + 1 < argc))
        {
            this->port = std::stoi(argv[i + 1]);
            ++i; // Пропустить значение параметра
        }
        else if ((strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--address") == 0) && (i + 1 < argc))
        {
            this->address = argv[i + 1];
            ++i; // Пропустить значение параметра
        }
        else
        {
            throw ArgsDecodeException("Invalid argument", "Interface::parseArgs", this->log_path, true);
        }
    }

    if (this->db_path.empty())
        this->db_path = "/etc/vcalc.conf";

    if (this->log_path.empty())
        this->log_path = "/var/log/vcalc.log";

    if (this->address.empty())
        this->address = "127.0.0.1";

    if (this->port == 0)
        this->port = 33333;

    if (this->port <= 0 || this->port > 65535)
        throw ArgsDecodeException("Invalid port number", "Interface::parseArgs", this->log_path, true);

    std::cout << "Parsed arguments: --path_to_database " << this->db_path
              << " --path_to_log " << this->log_path
              << " --address " << this->address
              << " --port " << this->port << std::endl;
}

// Метод для чтения базы данных паролей и записи в атрибут map
void Interface::readDB()
{
    std::ifstream input_file(this->db_path);
    if (!input_file.is_open())
        throw IOException("Failed to open database file for reading", "Interface::readDB", this->log_path, true);

    std::string line;

    while (std::getline(input_file, line))
    {
        auto delimiter_pos = line.find(':');
        if (delimiter_pos != std::string::npos)
        {
            std::string username = line.substr(0, delimiter_pos);
            std::string password = line.substr(delimiter_pos + 1);
            this->database[username] = password;
        }
        else
            throw DataDecodeException("Invalid line format in database file", "Interface::readDB", this->log_path, true);
    }

    if (input_file.bad())
        throw DataDecodeException("Error reading database file", "Interface::readDB", this->log_path, true);

    input_file.close();

    std::cout << "Read database from file: " << this->db_path << std::endl;
}

// Метод для вывода справки
void Interface::showHelp() const
{
    std::cout << "Usage: program [options]\nOptions:\n"
              << "  -h, --help                 Show this help message and exit\n"
              << "  -pdb, --path_to_database   Path to the database file\n"
              << "  -pl, --path_to_log         Path to the log file\n"
              << "  -p, --port                 Port number (default: 33333)\n"
              << "  -a, --address              Address (default: 127.0.0.1)\n";
}

// Метод для запуска сервера
void Interface::run()
{
    while (true)
    {
        try
        {
            this->network->wait();
            this->network->auth();
            this->network->calc();
            this->network->close();
        }
        catch (const NetworkException &e)
        {
            this->network->close();
            std::cerr << e.what() << std::endl;
        }
        catch (const std::exception &e)
        {
            this->network->quit();
            throw Exception("Exception", "undefined", "Interface::run", this->log_path);
        }
    }
}
