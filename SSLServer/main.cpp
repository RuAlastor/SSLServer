#include "server.h"
#include "slave.h"

int main() {
    Sorokin::Server _server;

    if (_server.setSocket()) {
        return -1;
    }
    if (_server.setUpListener()) {
        try {
            _server.deleteSocket();
        }
        catch (std::exception& error) {
            std::cout << error.what() << '\n';
        }
        return -1;
    }
    Sorokin::Slave _slave;
    _server.getConnection(_slave.accessSocket());
    _slave.closeConnection();

    _server.closeConnection();

    try {
        _slave.deleteSocket();
        _server.deleteSocket();
    }
    catch (std::exception& error) {
        std::cout << error.what() << '\n';
        return -1;
    }

    return 0;
}

