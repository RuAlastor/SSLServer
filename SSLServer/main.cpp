#include "sockets.h"

int main() {

    Sorokin::Socket _server;
    if (_server.setUpSocket() || _server.setUpSocketInfo()) {
        return -1;
    }

    try {
        _server.deleteSocketInfo();
        _server.closeDescriptor();
    }
    catch (std::exception& error) {
        std::cout << error.what() << '\n';
        return -1;
    }

    return 0;
}

