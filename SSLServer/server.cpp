#include "server.h"

using namespace Sorokin;

/*
// SERVER

Server::Server() noexcept : _masterSocketPtr(nullptr), _slaveSocektPtr(nullptr), _pwd(nullptr) {}
Server::~Server() noexcept {
    delete _masterSocketPtr;
    _masterSocketPtr = nullptr;
    delete _slaveSocektPtr;
    _slaveSocektPtr = nullptr;
    delete _pwd;
    _pwd = nullptr;
}

void Server::Clear() noexcept {
    delete _masterSocketPtr;
    _masterSocketPtr = nullptr;
    delete _slaveSocektPtr;
    _slaveSocektPtr = nullptr;
    delete _pwd;
    _pwd = nullptr;
}

void Server::CreateMasterSocket(int port) noexcept(false) {
    _masterSocketPtr = new MasterSocket(port);
    try {
        _masterSocketPtr->Start();
    }
    catch (std::exception& error) {
        _masterSocketPtr->CloseConnection();
        std::cout << error.what() << '\n';
        throw MasterSocketException();
    }
}

void Server::WaitForConnection() noexcept(false) {
    try {
        SlaveSocketInfo info = _masterSocketPtr->Handle();
        _slaveSocektPtr = new SlaveSocket(info._fd, _pwd, info._slaveSocketInfo, info._slaveSocketInfoLen);
        this->GetSignSend();
    }
    catch (std::exception& error) {
        _masterSocketPtr->CloseConnection();
        std::cout << error.what() << '\n';
        throw MasterSocketException();
    }
}

void Server::GetSignSend() noexcept(false) {
    try {
        _slaveSocektPtr->RecvFile();
        _slaveSocektPtr->SignFile();
        _slaveSocektPtr->SendFile();
    }
    catch (std::exception& error) {
        _slaveSocektPtr->CloseConnection();
        std::cout << error.what() << '\n';
        throw SlaveSocketException();
    }
}
*/
