#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>

class TCPServer {
public:
    TCPServer(int port);
    ~TCPServer();
    void Start();

private:
    int serverSocket;
    sockaddr_in serverAddr;
    sockaddr_in clientAddr; // Declare clientAddr here
    socklen_t clientAddrLen;
    std::vector<std::thread> clientThreads;

    void HandleClient(int clientSocket);
};

TCPServer::TCPServer(int port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port << "..." << std::endl;

    clientAddrLen = sizeof(clientAddr);
}

TCPServer::~TCPServer() {
    close(serverSocket);
}

void TCPServer::Start() {
    while (true) {
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

        clientThreads.emplace_back(&TCPServer::HandleClient, this, clientSocket);
        clientThreads.back().detach();
    }
}

void TCPServer::HandleClient(int clientSocket) {
    char buffer[1024];
    while (true) {
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }

        buffer[bytesRead] = '\0';
        std::cout << "Received from client: " << buffer << std::endl;

        if (strcmp(buffer, "bye") == 0) {
            std::cout << "Client sent 'bye'. Replying with 'Goodbye'" << std::endl;
            send(clientSocket, "Goodbye", strlen("Goodbye"), 0);
            break;
        } else {
            std::cout << "Client sent: " << buffer << ". Replying with 'Ok'" << std::endl;
            send(clientSocket, "Ok", strlen("Ok"), 0);
        }
    }
    close(clientSocket);
}

int main() {
    TCPServer server(6000);
    server.Start();

    return 0;
}
