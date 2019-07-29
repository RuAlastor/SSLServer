#include "server.h"
#include "slave.h"
#include "parser.h"
#include "signer.h"


int main() {
    Sorokin::Signer _signer;
    _signer.createKeyPair();

    /*
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

                if (!_xmlParser.loadDocument(someString)) {
                    // Document was succesfully loaded
                    std::list<std::string>* someList = nullptr;

                    try {
                        someList = _xmlParser.parseDocument();
                    }
                    catch (std::exception& error) {
                        std::cout << error.what() << '\n';
                    }
                    if (someList != nullptr) {
                        // Document was succesfully parsed

                        _slave.sendString(someString);
                    }

                    delete someList;
                }

                _xmlParser.unloadDocument();
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
    */

    return 0;
}

