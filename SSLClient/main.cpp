#include "client.h"

int main() {
    Sorokin::Client client("/home/student/C++/unsignedXML.xml", 12345);
    if (client.Start()) {
        return -1;
    }
    if (client.Handle()) {
        return -1;
    }

    return 0;
}
