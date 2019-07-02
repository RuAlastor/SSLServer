#include "masterserver.h"

using namespace Sorokin;


MasterSocket::MasterSocket(int port) noexcept : _ip(INADDR_LOOPBACK),
                                                _port(port),
                                                _masterSocket(-1) {}
MasterSocket::~MasterSocket() {
    if (_masterSocket != -1) {
        shutdown(_masterSocket, SHUT_RDWR);
        close(_masterSocket);
    }
}

int MasterSocket::Start() noexcept {
    // Initialize server
    _masterSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_masterSocket == -1) {
        std::cout << "Failed to initialize listening socket!\n";
        return socketInitError;
    }
    std::cout << "Socket was initialized!\n";

    // Set socket info
    sockaddr_in socketInfo;
    memset(&socketInfo.sin_zero, 0, sizeof(socketInfo.sin_zero));
    socketInfo.sin_family = AF_INET;
    socketInfo.sin_addr.s_addr = htonl(_ip);
    socketInfo.sin_port = htons(_port);

    // Bind socket
    if (bind(_masterSocket, reinterpret_cast<sockaddr*>(&socketInfo), sizeof(socketInfo)) == -1) {
        std::cout << "Failed to bind socket!\n";
        return socketBindError;
    }
    std::cout << "Socket was binded!\n";

    // Set to listen
    if (listen(_masterSocket, SOMAXCONN) == -1) {
        std::cout << "Failed to set socket to listen!\n";
        return socketListenError;
    }
    std::cout << "Socket was set to listen!\n";

    return noError;
}

int MasterSocket::Handle() noexcept(false) {
    std::cout << "Waiting for connection!\n"; //

    int slaveSocket = accept(_masterSocket, nullptr, nullptr);
    if (slaveSocket < 0) {
        std::cout << "Connection error!\n";
        return connectionError;
    }
    SlaveSocket slaveSocketObj(slaveSocket, "/home/student/C++/localSignedXML.xml", nullptr, nullptr);
    slaveSocketObj.Start();

    return noError;
}



SlaveSocket::SlaveSocket(int fd,
                         const char* filename,
                         sockaddr_in* socketInfo,
                         socklen_t* socketInfoLen) noexcept : _slaveSocket(fd),
                                                              _filename(filename),
                                                              _socketInfo(socketInfo),
                                                              _socketInfoLen(socketInfoLen) {}

SlaveSocket::~SlaveSocket() {
    shutdown(_slaveSocket, SHUT_RDWR);
    close(_slaveSocket);
}

int SlaveSocket::Start() noexcept {
    std::cout << "Got a connection!\n";

    char buffer[1024];
    std::ofstream xmlWriter;
    xmlWriter.open(_filename);
    if (!xmlWriter.is_open()) {
        std::cout << "Can't open the file!\n";
        return fileWritingError;
    }

    while (true) {
        int rResult = recv(_slaveSocket, buffer, sizeof(char) * 1024, 0);
        if (rResult == -1) {
            std::cout << "Connection stopped!\n";
            break;
        }
        if (rResult == 0) {
            std::cout << "Got the document!\n";
            break;
        }
        xmlWriter << buffer << std::endl;
        memset(buffer, 0, sizeof(char) * 1024);
    }

    return noError;
}





