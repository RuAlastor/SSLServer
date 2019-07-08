#include "masterserver.h"

int main() {

    Sorokin::MasterSocket server(12345);
    server.AskPwd();
    try {
        server.Start();
        server.Handle();
    }
    catch (std::exception error) {
        std::cout << error.what();
        return -1;
    }

    return 0;
}

