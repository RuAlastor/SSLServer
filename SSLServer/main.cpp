#include "server.h"
#include "slave.h"
#include "parser.h"


int main() {
    Sorokin::Server _server;
    if (_server.setSocket()) {
        return -1;
    }

    // Socket was succesfully created
    if (!_server.setUpListener()) {
        // Socket was succesfully binded
        Sorokin::Slave _slave;
        if (!_server.getConnection(_slave.accessSocket())) {
            // Connection was succesfully accepted
            std::string someString = "";
            if (!_slave.recvString(someString)) {
                // Document was succesfully recieved
                Sorokin::Parser _xmlParser;

                std::list<std::string>* someList = _xmlParser.parseDocument(someString);
                // Document was succesfully parsed
                if (someList != nullptr) {

                    _slave.sendString(someString);
                }
                delete someList;
                _xmlParser.tryToCleanUp();
            }

            _slave.closeConnection();
        }
        try {
            _slave.deleteSocket();
        }
        catch (std::exception& error) {
            std::cout << error.what() << '\n';
        }
        _server.closeConnection();
    }

    try {
        _server.deleteSocket();
    }
    catch (std::exception& error) {
        std::cout << error.what() << '\n';
        return -1;
    }
    catch (...) {
        std::cout << "Unknown error!\n";
        return -1;
    }

    return 0;
}

