#ifndef MASTERSOCKETEXCEPTIONS_H
#define MASTERSOCKETEXCEPTIONS_H

#include "headers.h"

namespace Sorokin {

    class MasterSocketException : public std::exception {

    public:
        MasterSocketException() = default;
        virtual ~MasterSocketException() = default;

        MasterSocketException(const MasterSocketException& other) = delete;
        MasterSocketException(MasterSocketException&& other) = delete;
        MasterSocketException& operator=(const MasterSocketException& other) = delete;
        MasterSocketException& operator=(MasterSocketException&& other) = delete;

        inline virtual const char* what() const throw() {
            return "Master-socket error!\n";
        }

    };

    class MasterSocketInitError : public MasterSocketException {

    public:
        MasterSocketInitError() = default;
        ~MasterSocketInitError() = default;

        MasterSocketInitError(const MasterSocketInitError& other) = delete;
        MasterSocketInitError(MasterSocketInitError&& other) = delete;
        MasterSocketInitError& operator=(const MasterSocketInitError& other) = delete;
        MasterSocketInitError& operator=(MasterSocketInitError&& other) = delete;

        inline const char* what() const throw() {
            return "Failed to initialize listening socket!\n";
        }

    };

    class MasterSocketBindError : public MasterSocketException {

    public:
        MasterSocketBindError() = default;
        ~MasterSocketBindError() = default;

        MasterSocketBindError(const MasterSocketBindError& other) = delete;
        MasterSocketBindError(MasterSocketBindError&& other) = delete;
        MasterSocketBindError& operator=(const MasterSocketBindError& other) = delete;
        MasterSocketBindError& operator=(MasterSocketBindError&& other) = delete;

        inline const char* what() const throw() {
            return "Failed to bind socket!\n";
        }
    };

    class MasterSocketListenError : public MasterSocketException {

    public:
        MasterSocketListenError() = default;
        ~MasterSocketListenError() = default;

        MasterSocketListenError(const MasterSocketListenError& other) = delete;
        MasterSocketListenError(MasterSocketListenError&& other) = delete;
        MasterSocketListenError& operator=(const MasterSocketListenError& other) = delete;
        MasterSocketListenError& operator=(MasterSocketListenError&& other) = delete;

        inline const char* what() const throw() {
            return "Failed to set socket to listen!\n";
        }
    };

}

namespace Sorokin {

class SlaveSocketException : public std::exception {

public:
    SlaveSocketException() = default;
    virtual ~SlaveSocketException() = default;

    SlaveSocketException(const SlaveSocketException& other) = delete;
    SlaveSocketException(SlaveSocketException&& other) = delete;
    SlaveSocketException& operator=(const SlaveSocketException& other) = delete;
    SlaveSocketException& operator=(SlaveSocketException&& other) = delete;

    inline virtual const char* what() const throw() {
        return "Slave-socket error!\n";
    }

};

class SlaveSocketConnectionError : public SlaveSocketException {

public:
    SlaveSocketConnectionError() = default;
    virtual ~SlaveSocketConnectionError() = default;

    SlaveSocketConnectionError(const SlaveSocketConnectionError& other) = delete;
    SlaveSocketConnectionError(SlaveSocketConnectionError&& other) = delete;
    SlaveSocketConnectionError& operator=(const SlaveSocketConnectionError& other) = delete;
    SlaveSocketConnectionError& operator=(SlaveSocketConnectionError&& other) = delete;

    inline const char* what() const throw() {
        return "Failed to connect client!\n";
    }

};

class SlaveSocketRecievingError : public SlaveSocketException {

public:
    SlaveSocketRecievingError() = default;
    ~SlaveSocketRecievingError() = default;

    SlaveSocketRecievingError(const SlaveSocketRecievingError& other) = delete;
    SlaveSocketRecievingError(SlaveSocketRecievingError&& other) = delete;
    SlaveSocketRecievingError& operator=(const SlaveSocketRecievingError& other) = delete;
    SlaveSocketRecievingError& operator=(SlaveSocketRecievingError&& other) = delete;

    inline const char* what() const throw() {
        return "Failed to recieve data!\n";
    }

};

}


#endif // MASTERSOCKETEXCEPTIONS_H
