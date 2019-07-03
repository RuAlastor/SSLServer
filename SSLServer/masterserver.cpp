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

    SlaveSocket slaveSocketObj(accept(_masterSocket, nullptr, nullptr),
                               &_pwd,
                               "/home/student/C++/localSignedXML.xml",
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
                         const char* filename,
                         sockaddr_in* socketInfo,
                         socklen_t* socketInfoLen) noexcept : _slaveSocket(fd),
                                                              _pwd(pwd),
                                                              _filename(filename),
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

    // Check file
    std::ofstream xmlWriter;
    xmlWriter.open(_filename);
    if (!xmlWriter.is_open()) {
        std::cout << "Can't open the file!\n";
        return fileWritingError;
    }

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
    std::cout << "Got the document!\n";

    // Write it to the file
    xmlWriter << buffer << std::endl;
    xmlWriter.close();
    delete[] buffer;

    return noError;
}

int SlaveSocket::SignFile() noexcept(false) {
    std::list<std::string> strsToSign;

    Parser xmlParser(_filename, &strsToSign);
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

    // Send size of the doc
    int sResult = send(_slaveSocket, reinterpret_cast<const char*>(&fileSize), sizeof(int), 0);
    if (sResult <= 0) {
        std::cout << "Failed to send data!\n";
        return sendError;
    }

    // Send doc
    std::string text = "";
    std::string strBuffer = "";
    while (!xmlReader.eof()) {
        strBuffer.clear();
        std::getline(xmlReader, strBuffer);
        text += strBuffer + '\n';
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



