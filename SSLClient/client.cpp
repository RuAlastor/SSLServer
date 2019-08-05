#include "client.h"

using namespace Sorokin;

/*
Client::Client(const char* filenameOut, const char* filenameIn, int port) noexcept : _filenameOut(filenameOut),
                                                                                     _filenameIn(filenameIn),
                                                                                     _ip(INADDR_LOOPBACK),
                                                                                     _port(port),
                                                                                     _masterSocket(-1) {}

Client::~Client() noexcept {
    shutdown(_masterSocket, SHUT_RDWR);
    close(_masterSocket);
}

void Client::CloseConnect() noexcept {
    shutdown(_masterSocket, SHUT_RDWR);
    close(_masterSocket);
}

void Client::Start() noexcept(false) {
    // Try to connect
    _masterSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_masterSocket == -1) {
        throw ClientInitError();
    }
    std::cout << "Socket was initialized!\n";

    sockaddr_in socketInfo;
    memset(&socketInfo.sin_zero, 0, sizeof(socketInfo.sin_zero));
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(_ip);
    socketInfo.sin_port = htons(_port);

    if (connect(_masterSocket, reinterpret_cast<sockaddr*>(&socketInfo), sizeof(socketInfo))) {
        throw ClientConnectError();
    }
    std::cout << "Succesfully connected to server!\n";
}

void Client::Handle() noexcept(false) {
    this->Send();
    this->Get();
}

void Client::Send() noexcept(false) {
    // Check file
    std::ifstream xmlReader;
    xmlReader.open(_filenameOut, std::ios_base::ate | std::ios_base::binary);
    if (!xmlReader.is_open()) {
        throw FileError();
    }
    const int fileSize = xmlReader.tellg();
    std::cout << fileSize << std::endl;
    xmlReader.seekg(0, std::ios_base::beg);

    // Send file to the server
    int sResult = send(_masterSocket, reinterpret_cast<const char*>(&fileSize), sizeof(int), 0);
    if (sResult <= 0) {
        throw ClientSendError();
    }

    std::string text = "";
    std::string strBuffer = "";
    while (!xmlReader.eof()) {
        strBuffer.clear();
        std::getline(xmlReader, strBuffer);
        text += strBuffer + '\n';
    }
    sResult = send(_masterSocket, text.c_str(), fileSize, 0);
    if (sResult <= 0) {
        throw ClientSendError();
    }

    // Close reading file
    xmlReader.close();
}

void Client::Get() noexcept(false) {
    std::ofstream xmlWriter;
    xmlWriter.open(_filenameIn, std::ios::binary | std::ios::trunc);
    if (!xmlWriter.is_open()) {
        throw FileError();
    }

    uint32_t bufSize = 0;
    int rResult = recv(_masterSocket, reinterpret_cast<char*>(&bufSize), sizeof(uint32_t), 0);
    if (rResult <0) {
        throw ClientRecvError();
    }
    char* buffer = new char[bufSize];
    rResult = recv(_masterSocket, buffer, bufSize, 0);
    if (rResult <0) {
        throw ClientRecvError();
    }

    std::cout << "Bytes recieved: " << rResult << '\n'
              << "Message: ";

    std::cout << "Got the document!\n";

    std::cout << "Bytes recieved: " << rResult << '\n';
    for (uint32_t i = 0; i < bufSize; i++) {
        std::cout << buffer[i];
    }

    xmlWriter.close();
    delete[] buffer;

    xmlWriter.close();
}

*/





















