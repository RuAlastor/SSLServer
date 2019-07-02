#include "client.h"

using namespace Sorokin;

Client::Client(const char* filename, int port) noexcept : _filename(filename),
                                                          _ip(INADDR_LOOPBACK),
                                                          _port(port),
                                                          _masterSocket(-1) {}

Client::~Client() noexcept {
    if (_masterSocket != -1) {
        shutdown(_masterSocket, SHUT_RDWR);
        close(_masterSocket);
    }
}

int Client::Start() noexcept {
    // Try to connect
    _masterSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_masterSocket == -1) {
        std::cout << "Failed to initialize socket!\n";
        return socketInitError;
    }
    std::cout << "Socket was initialized!\n";

    sockaddr_in socketInfo;
    memset(&socketInfo.sin_zero, 0, sizeof(socketInfo.sin_zero));
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(_ip);
    socketInfo.sin_port = htons(_port);

    if (connect(_masterSocket, reinterpret_cast<sockaddr*>(&socketInfo), sizeof(socketInfo))) {
        std::cout << "Failed to connect!\n";
        return connectionError;
    }
    std::cout << "Succesfully connected to server!\n";

    return noErrors;
}

int Client::Handle() noexcept(false) {
    // Check file
    std::ifstream xmlReader;
    xmlReader.open(_filename);
    if (!xmlReader.is_open()) {
        std::cout << "Failed to open file!\n";
        return fileOpeningError;
    }
    std::cout << "File is ready to read!\n";

    // Send file to the server
    std::string buffer;
    std::getline(xmlReader, buffer);
    int sResult;
    while (!xmlReader.eof()) {
        std::cout << buffer << '\n'; // Delete later
        sResult = send(_masterSocket, buffer.c_str(), buffer.size(), 0);
        if (sResult <= 0) {
            std::cout << "Failed to send data!\n";
            xmlReader.close();
            return sendError;
        }
        std::getline(xmlReader, buffer);
    }

    // Get data here

    return noErrors;
}

























