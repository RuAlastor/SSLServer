#ifndef MASTERSOCKETEXCEPTIONS_H
#define MASTERSOCKETEXCEPTIONS_H

#include "headers.h"

namespace Sorokin {

    class MasterSocketException : public std::exception {
    public:
        inline virtual const char* what() const throw() {
            return "MasterSocket error!\n";
        }
    };
    class masterSocketInitError : public MasterSocketException {
    public:
        inline const char* what() const throw() {
            return "Failed to initialize listening socket!\n";
        }
    };
    class masterSocketBindError : public MasterSocketException {
        inline const char* what() const throw() {
            return "Failed to bind socket!\n";
        }
    };
    class masterSocketListenError : public MasterSocketException {
        inline const char* what() const throw() {
            return "Failed to set socket to listen!\n";
        }
    };

    class SlaveSocketException : public std::exception {
    public:
        inline virtual const char* what() const throw() {
            return "SlaveSocket error!\n";
        }
    };

}

#endif // MASTERSOCKETEXCEPTIONS_H
