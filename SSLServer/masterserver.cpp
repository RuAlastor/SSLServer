#include "masterserver.h"

using namespace Sorokin;


// MASTER SOCKET

MasterSocket::MasterSocket(int port) noexcept : _ip(INADDR_LOOPBACK),
                                                _port(port),
                                                _masterSocket(-1),
                                                _pwd("12345") {}
MasterSocket::~MasterSocket() noexcept {
    if (_masterSocket != -1) {
        shutdown(_masterSocket, SHUT_RDWR);
        close(_masterSocket);
    }
}

void MasterSocket::AskPwd() noexcept(false) {
    std::cout << "Enter the password for the private key: ";
    _pwd.clear();
    std::cin >> _pwd;
}

void MasterSocket::Start() noexcept(false) {
    // Initialize server
    _masterSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_masterSocket == -1) {
        throw masterSocketInitError();
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
        throw masterSocketBindError();
    }
    std::cout << "Socket was binded!\n";

    // Set to listen
    if (listen(_masterSocket, SOMAXCONN) == -1) {
        throw masterSocketListenError();
    }
    std::cout << "Socket was set to listen!\n";
}

int MasterSocket::Handle() noexcept(false) {
    std::cout << "Waiting for connection...\n";

    SlaveSocket slaveSocketObj(accept(_masterSocket, nullptr, nullptr),
                               &_pwd,
                               nullptr,
                               nullptr);
    if (slaveSocketObj.RecvFile()) {
        return slaveSocketError;
    }
    if (slaveSocketObj.SignFile()) {
        return slaveSocketError;
    }
    if (slaveSocketObj.SendFile()) {
        return slaveSocketError;
    }

    return noError;
}

// SLAVE SOCKET

SlaveSocket::SlaveSocket(int fd,
                         const std::string* pwd,
                         sockaddr_in* socketInfo,
                         socklen_t* socketInfoLen) noexcept : _slaveSocket(fd),
                                                              _pwd(pwd),
                                                              _socketInfo(socketInfo),
                                                              _socketInfoLen(socketInfoLen) {}

SlaveSocket::~SlaveSocket() {
    shutdown(_slaveSocket, SHUT_RDWR);
    close(_slaveSocket);
}

int SlaveSocket::RecvFile() noexcept {
    if (_slaveSocket < 0) {
        std::cout << "Connection error!\n";
        return connectionError;
    }
    std::cout << "Got a connection!\n";

    // Get size of the doc
    char* buffer = new char[sizeof(int)];
    int rResult = recv(_slaveSocket, buffer, sizeof(int), 0);
    if (rResult <0) {
        std::cout << "Failed to recieve message!\n";
        return acceptError;
    }
    int fileSize = 0;
    memcpy(&fileSize, buffer, sizeof(int));
    delete[] buffer;

    // Get the doc
    buffer = new char[fileSize];
    rResult = recv(_slaveSocket, buffer, fileSize, 0);
    if (rResult <0) {
        std::cout << "Failed to recieve message!\n";
        return acceptError;
    }
    _xmlFileIn.clear();
    for (int i = 0; i < fileSize; i++) {
        _xmlFileIn += buffer[i];
    }
    std::cout << "Got the document!\n";

    delete[] buffer;

    return noError;
}

int SlaveSocket::SignFile() noexcept(false) {
    std::list<std::string> strsToSign;

    Parser xmlParser(&_xmlFileIn, &strsToSign);
    if (xmlParser.loadDocument()) {
        return parseError;
    }
    if (xmlParser.parseDocument()) {
        return parseError;
    }

    Signer signer(*_pwd);

    std::list<std::string> signedStrs;
    for (const auto iter : strsToSign) {
        signedStrs.push_back(signer.SignString(iter));
    }
    std::string publicKey = signer.GetPublicKey();

    xmlParser.rebuildDocument(signedStrs, publicKey);

    return noError;
}

int SlaveSocket::SendFile() noexcept {
    // Send size of the doc
    int fileSize = _xmlFileIn.size();
    std::cout << fileSize << '\n';
    int sResult = send(_slaveSocket, reinterpret_cast<const char*>(&fileSize), sizeof(int), 0);
    if (sResult <= 0) {
        std::cout << "Failed to send size!\n";
        return sendError;
    }

    // Send doc
    sResult = send(_slaveSocket, _xmlFileIn.c_str(), fileSize, 0);
    if (sResult <= 0) {
        std::cout << "Failed to send data!\n";
        return sendError;
    }

    return noError;
}



