#include "masterserver.h"

int main() {

    Sorokin::MasterSocket server(12345);
    server.AskPwd();
    try {
        server.Start();
        server.Handle();
    }
    catch (std::exception& error) {
        std::cout << error.what();
    }
    server.End();

    return 0;
}

