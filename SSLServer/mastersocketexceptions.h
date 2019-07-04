#ifndef MASTERSOCKETEXCEPTIONS_H
#define MASTERSOCKETEXCEPTIONS_H

#include "headers.h"

namespace Sorokin {

    class MasterSocketException : public std::exception {

    };
    class masterSocketInitError : public MasterSocketException {
    public:
        inline const char* what() const {
            return "Failed to initialize listening socket!\n";
        }
    };
    class masterSocketBindError : public MasterSocketException {
        inline const char* what() const {
            return "Failed to bind socket!\n";
        }
    };
    class masterSocketListenError : public MasterSocketException {
        inline const char* what() const {
            return "Failed to set socket to listen!\n";
        }
    };

}

#endif // MASTERSOCKETEXCEPTIONS_H
