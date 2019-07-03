#include "masterserver.h"

using namespace Sorokin;


// MASTER SOCKET

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
    std::cout << "Waiting for connection...\n";

    int slaveSocket = accept(_masterSocket, nullptr, nullptr);
    if (slaveSocket < 0) {
        std::cout << "Connection error!\n";
        return connectionError;
    }

    SlaveSocket slaveSocketObj(slaveSocket,
                               "/home/student/C++/localSignedXML.xml",
                               nullptr,
                               nullptr);
    if (slaveSocketObj.Start()) {
        return slaveSocketError;
    }
    if (slaveSocketObj.SignFile()) {
        return slaveSocketError;
    }
    if (slaveSocketObj.SendBack()) {
        return slaveSocketError;
    }

    return noError;
}

// SLAVE SOCKET

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

    std::ofstream xmlWriter;
    xmlWriter.open(_filename);
    if (!xmlWriter.is_open()) {
        std::cout << "Can't open the file!\n";
        return fileWritingError;
    }

    char* buffer = new char[sizeof(int)];
    int rResult = recv(_slaveSocket, buffer, sizeof(int), 0);
    if (rResult <0) {
        std::cout << "Failed to recieve message!\n";
        return acceptError;
    }
    int fileSize = 0;
    memcpy(&fileSize, buffer, sizeof(int));
    delete[] buffer;
    buffer = new char[fileSize];
    rResult = recv(_slaveSocket, buffer, fileSize, 0);
    if (rResult <0) {
        std::cout << "Failed to recieve message!\n";
        return acceptError;
    }

    std::cout << "Got the document!\n";

    xmlWriter << buffer << std::endl;
    xmlWriter.close();
    delete[] buffer;

    return noError;
}

int SlaveSocket::SignFile() noexcept(false) {
    std::list<std::string> strsToSign;
    Parser xmlParser(_filename, &strsToSign);

    try {
        xmlParser.loadDocument();
    }
    catch (...) {
        std::cout << "Document is fucked!\n";
        return parseError;
    }

    if (xmlParser.parseDocument()) {
        return parseError;
    }

    Signer signer("/home/student/C++/public_key", "/home/student/C++/private_key");
    signer.GetAccess();

    std::list<unsigned char*> signedStrs;
    for (const auto iter : strsToSign) {
        signedStrs.push_back(signer.SignString(iter));
    }
    std::string publicKey = signer.GetPublicKey();

    xmlParser.rebuildDocument(signedStrs, publicKey);

    for (const auto iter : signedStrs) {
        delete[] iter;
    }
    return noError;
}

int SlaveSocket::SendBack() noexcept {
    // Check file
    std::ifstream xmlReader;
    xmlReader.open(_filename, std::ios_base::ate | std::ios_base::binary);
    if (!xmlReader.is_open()) {
        std::cout << "Failed to open file!\n";
        return -1;
    }
    const int fileSize = xmlReader.tellg();
    std::cout << fileSize << std::endl;
    xmlReader.seekg(0, std::ios_base::beg);

    // Send file to the server
    int sResult = send(_slaveSocket, reinterpret_cast<const char*>(&fileSize), sizeof(int), 0);
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
    sResult = send(_slaveSocket, text.c_str(), fileSize, 0);
    if (sResult <= 0) {
        std::cout << "Failed to send data!\n";
        return sendError;
    }

    // Close reading file
    xmlReader.close();

    return noError;
}



