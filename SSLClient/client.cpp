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
    xmlReader.open(_filename, std::ios_base::ate | std::ios_base::binary);
    if (!xmlReader.is_open()) {
        std::cout << "Failed to open file!\n";
        return fileOpeningError;
    }
    const int fileSize = xmlReader.tellg();
    std::cout << fileSize << std::endl;
    xmlReader.seekg(0, std::ios_base::beg);

    // Send file to the server
    int sResult = send(_masterSocket, reinterpret_cast<const char*>(&fileSize), sizeof(int), 0);
    if (sResult <= 0) {
        std::cout << "Failed to send data!\n";
        return sendError;
    }

    std::string text = "";
    std::string strBuffer = "";
    while (!xmlReader.eof()) {
        strBuffer.clear();
        std::getline(xmlReader, strBuffer);
        text += strBuffer;
    }
    sResult = send(_masterSocket, text.c_str(), fileSize, 0);
    if (sResult <= 0) {
        std::cout << "Failed to send data!\n";
        return sendError;
    }

    // Close reading file
    xmlReader.close();

    // Get data here
    std::ofstream xmlWriter;
    xmlWriter.open("/home/student/C++/signedXML.xml", std::ios::binary | std::ios::trunc);
    if (!xmlWriter.is_open()) {
        std::cout << "Can't open the file!\n";
        return -1;
    }

    char* buffer = new char[sizeof(int)];
    int rResult = recv(_masterSocket, buffer, sizeof(int), 0);
    if (rResult <0) {
        std::cout << "Failed to recieve message!\n";
        return -1;
    }
    int fileSizeRecieved = 0;
    memcpy(&fileSizeRecieved, buffer, sizeof(int));
    delete[] buffer;
    buffer = new char[fileSizeRecieved];
    rResult = recv(_masterSocket, buffer, fileSizeRecieved, 0);
    if (rResult <0) {
        std::cout << "Failed to recieve message!\n";
        return -1;
    }

    std::cout << "Got the document!\n";
    std::cout << rResult << '\n';

    for (int i = 0; i < fileSizeRecieved; i++) {
        xmlWriter << buffer[i];
    }
    xmlWriter.close();
    delete[] buffer;

    xmlWriter.close();
    return noErrors;
}

























