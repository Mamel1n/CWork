#include "network.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdint>
#include <vector>

// Конструктор
Network::Network(const std::string &address, uint16_t port,
                 const std::map<std::string, std::string> &database,
                 const std::string &log_path)
    : address(address), port(port), database(database),
      log_path(log_path), socket(-1), client_socket(-1)
{
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket == -1)
    {
        throw NetworkException("Failed to create socket", "Network::Network", this->log_path);
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->port);
    server_addr.sin_addr.s_addr = inet_addr(this->address.c_str());

    if (bind(this->socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        ::close(this->socket);
        throw NetworkException("Failed to bind socket", "Network::Network", this->log_path);
    }

    if (listen(this->socket, 5) == -1)
    {
        ::close(this->socket);
        throw NetworkException("Failed to listen on socket", "Network::Network", this->log_path);
    }
}

std::string &Network::getAddress()
{
    return this->address;
};

uint16_t &Network::getPort()
{
    return this->port;
};

std::map<std::string, std::string> &Network::getDatabase()
{
    return this->database;
};

// Метод для ожидания соединений
void Network::wait()
{
    std::cout << "Server listening on " << this->address << ":" << this->port << std::endl;
    this->client_socket = accept(this->socket, nullptr, nullptr);
    if (this->client_socket == -1)
    {
        throw NetworkException("Failed to accept connection", "Network::wait", this->log_path);
    }
    std::cout << "Accepted connection from client" << std::endl;
}

// Метод для аутентификации
void Network::auth()
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytes_read = recv(this->client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0)
    {
        throw NetworkException("Failed to read login from client", "Network::auth", this->log_path);
    }
    std::string login(buffer);
    std::cout << "Received login: " << login << std::endl;

    auto it = this->database.find(login);
    if (it == this->database.end())
    {
        std::string error_message = "ERR";
        send(this->client_socket, error_message.c_str(), error_message.size(), 0);
        throw NetworkException("Login not found in database: " + login, "Network::auth", this->log_path);
    }

    std::string salt = crutils::generateSalt();
    send(this->client_socket, salt.c_str(), salt.size(), 0);
    std::cout << "Sent salt: " << salt << std::endl;

    memset(buffer, 0, sizeof(buffer));
    bytes_read = recv(this->client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0)
    {
        throw NetworkException("Failed to read hash from client", "Network::auth", this->log_path);
    }
    std::string client_hash(buffer);
    std::cout << "Received hash: " << client_hash << std::endl;

    std::string password = it->second;
    std::string data_to_hash = salt + password;
    std::string server_hash = crutils::calculateHash(data_to_hash);
    std::cout << "Calculated hash: " << client_hash << std::endl;

    if (client_hash != server_hash)
    {
        std::string error_message = "ERR";
        send(this->client_socket, error_message.c_str(), error_message.size(), 0);
        throw NetworkException("Hash mismatch", "Network::auth", this->log_path);
    }
    std::string success_message = "OK";
    send(this->client_socket, success_message.c_str(), success_message.size(), 0);
    std::cout << "Client authenticated successfully" << std::endl;
}

// Метод для вычисления суммы векторов
void Network::calc()
{
    uint32_t num_vectors;
    uint32_t num_values;
    int64_t value;

    // Чтение количества векторов от клиента
    ssize_t bytes_read = recv(this->client_socket, &num_vectors, sizeof(num_vectors), 0);
    if (bytes_read <= 0)
    {
        std::cerr << "Error: Failed to read number of vectors from client\n";
        throw NetworkException("Failed to read number of vectors from client", "Network::calc", this->log_path);
    }
    std::cout << "Received number of vectors: " << num_vectors << std::endl;

    // Обработка каждого вектора
    for (uint32_t i = 0; i < num_vectors; ++i)
    {
        // Чтение количества значений в векторе от клиента
        bytes_read = recv(this->client_socket, &num_values, sizeof(num_values), 0);
        if (bytes_read <= 0)
        {
            std::cerr << "Error: Failed to read number of values from client, vector index: " << i << "\n";
            throw NetworkException("Failed to read number of values from client", "Network::calc", this->log_path);
        }
        std::cout << "Received number of values for vector " << i + 1 << ": " << num_values << std::endl;

        int64_t sum = 0;
        for (uint32_t j = 0; j < num_values; ++j)
        {
            // Чтение значения вектора от клиента
            bytes_read = recv(this->client_socket, &value, sizeof(value), 0);
            if (bytes_read <= 0)
            {
                std::cerr << "Error: Failed to read vector value from client, vector index: " << i << ", element index: " << j << "\n";
                throw NetworkException("Failed to read vector value from client", "Network::calc", this->log_path);
            }
            std::cout << "Received value for vector " << i + 1 << ", element " << j + 1 << ": " << value << std::endl;

            sum += value;
        }

        // Отправка суммы обратно клиенту
        bytes_read = send(this->client_socket, &sum, sizeof(sum), 0);
        if (bytes_read <= 0)
        {
            std::cerr << "Error: Failed to send sum to client, vector index: " << i << ", sum: " << sum << "\n";
            throw NetworkException("Failed to send sum to client", "Network::calc", this->log_path);
        }
        std::cout << "Sent sum for vector " << i + 1 << ": " << sum << std::endl;
    }

    std::cout << "Calculated and sent sums for all vectors" << std::endl;
}

// Метод для закрытия клиентского соединения
void Network::close()
{
    if (this->client_socket != -1)
    {
        if (shutdown(this->client_socket, SHUT_RDWR) == -1)
        {
            std::cerr << "Error: Failed to shutdown client socket\n";
        }
        if (::close(this->client_socket) == -1)
        {
            std::cerr << "Error: Failed to close client socket\n";
        }
        this->client_socket = -1;
        std::cout << "Closed client connection" << std::endl;
    }
}

// Метод для закрытия основного соединения
void Network::quit()
{
    this->close();
    if (this->socket != -1)
    {
        if (shutdown(this->socket, SHUT_RDWR) == -1)
        {
            std::cerr << "Error: Failed to shutdown server socket\n";
        }
        if (::close(this->socket) == -1)
        {
            std::cerr << "Error: Failed to close server socket\n";
        }
        this->socket = -1;
        std::cout << "Closed server socket" << std::endl;
    }
}
