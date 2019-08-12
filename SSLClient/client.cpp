#include "client.h"

using namespace Sorokin;

//-----------------------------------------------------------------------------------------------------------------------------

void Client::__printCError(std::string& preErrorMsg) noexcept(false) {
    error_t errorNum = errno;
    const size_t bufMsgSize = 256;
    char* bufMsg = new char[bufMsgSize];
    bufMsg[0] = '\0';
    char* errorMsg = nullptr;

    errorMsg = strerror_r(errorNum, bufMsg, bufMsgSize);
    std::cout << preErrorMsg << errorMsg << '\n';

    delete[] bufMsg;
    bufMsg = nullptr;
}

//-----------------------------------------------------------------------------------------------------------------------------

Client::err Client::start(const int domain      /* = AF_INET */,
                          const int type        /* = SOCK_STREAM */,
                          const int protocol    /* = 0 */,
                          const in_addr_t ip    /* = INADDR_LOOPBACK */,
                          const int port        /* = 12345 */
                          ) noexcept
{
    try { Slave::_slaveSocket = new Socket; }
    catch ( const std::bad_alloc& error)
    {
        std::cout << error.what() << '\n';
        return undefinedError;
    }
    catch (...)
    {
        std::cout << "Undefined error!\n";
        return undefinedError;
    }

    if ( Slave::_slaveSocket->setSocket( domain, type, protocol ) != Socket::noError ) return undefinedError;
    if ( Slave::_slaveSocket->setSocketInfo( domain, ip, port ) != Socket::noError )
    {
        Slave::_slaveSocket->closeDescriptor();
        return undefinedError;
    }

    if ( connect( Slave::_slaveSocket->getSocketfd(),
                  reinterpret_cast<const sockaddr*>( Slave::_slaveSocket->getSocketInfo() ),
                  sizeof( Slave::_slaveSocket->getSocketInfo() )
                  ) )
    {
        Slave::_slaveSocket->deleteSocketInfo();
        Slave::_slaveSocket->closeDescriptor();
        return undefinedError;
    }

    std::cout << "Connected to the server!\n";
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------


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





















