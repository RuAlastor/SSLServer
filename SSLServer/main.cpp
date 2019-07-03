#include "masterserver.h"

int main() {

    Sorokin::MasterSocket server(12345);
    server.AskPwd();
    if (server.Start()) {
        return -1;
    }
    if (server.Handle()) {
        return -1;
    }

    return 0;
}
