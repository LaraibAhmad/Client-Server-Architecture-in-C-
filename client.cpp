#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

class TCPClient {
public:
    TCPClient(const char* serverIP, int port);
    ~TCPClient();
    void Connect();
    void Start();

private:
    int clientSocket;
    sockaddr_in serverAddr;
    char message[1024];

    void SendMessage();
    void ReceiveMessage();
};

TCPClient::TCPClient(const char* serverIP, int port) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(port);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server" << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Connected to server" << std::endl;
}

TCPClient::~TCPClient() {
    close(clientSocket);
}

void TCPClient::SendMessage() {
    // Clear the buffer before reading input
    memset(message, 0, sizeof(message));

    std::cout << "Enter a message (or 'exit' to quit): ";
    std::cin.getline(message, sizeof(message));

    // Send message to server
    send(clientSocket, message, strlen(message), 0);
}

void TCPClient::ReceiveMessage() {
    // Clear the buffer before receiving the server's response
    memset(message, 0, sizeof(message));

    // Receive and display the server's response
    recv(clientSocket, message, sizeof(message), 0);
    std::cout << "Server response: " << message << std::endl;
}

void TCPClient::Connect() {
    while (true) {
        SendMessage();

        if (strcmp(message, "exit") == 0) {
            break;
        }

        ReceiveMessage();

        if (strcmp(message, "Goodbye") == 0) {
            std::cout << "Server sent 'Goodbye'. Closing the connection." << std::endl;
            break;
        }
    }
}

void TCPClient::Start() {
    Connect();

    // Close the client socket
    close(clientSocket);
}

int main() {
    TCPClient client("127.0.0.1", 6000);
    client.Start();

    return 0;
}
