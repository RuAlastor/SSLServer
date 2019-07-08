#include "server.h"

int main() {
    Sorokin::Server server;
    server.SetPwd("12345"); // Set password for private key
    try {
        server.CreateMasterSocket(12345); // Set port
        server.WaitForConnection();
    }
    catch (std::exception& error) {
        server.Clear();
        std::cout << error.what() << '\n';
    }

    return 0;
}

