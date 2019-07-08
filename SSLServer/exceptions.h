#ifndef MASTERSOCKETEXCEPTIONS_H
#define MASTERSOCKETEXCEPTIONS_H

#include "headers.h"

namespace Sorokin {

    class MasterSocketException : public std::exception {

    public:
        MasterSocketException() = default;
        virtual ~MasterSocketException() = default;
        MasterSocketException(const MasterSocketException& other) = default;
        MasterSocketException(MasterSocketException&& other) = default;

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
        MasterSocketInitError(const MasterSocketInitError& other) = default;
        MasterSocketInitError(MasterSocketInitError&& other) = default;

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
        MasterSocketBindError(const MasterSocketBindError& other) = default;
        MasterSocketBindError(MasterSocketBindError&& other) = default;

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
        MasterSocketListenError(const MasterSocketListenError& other) = default;
        MasterSocketListenError(MasterSocketListenError&& other) = default;

        MasterSocketListenError& operator=(const MasterSocketListenError& other) = delete;
        MasterSocketListenError& operator=(MasterSocketListenError&& other) = delete;

        inline const char* what() const throw() {
            return "Failed to set socket to listen!\n";
        }
    };

    class SlaveSocketException : public MasterSocketException {

    public:
        SlaveSocketException() = default;
        virtual ~SlaveSocketException() = default;
        SlaveSocketException(const SlaveSocketException& other) = default;
        SlaveSocketException(SlaveSocketException&& other) = default;

        SlaveSocketException& operator=(const SlaveSocketException& other) = delete;
        SlaveSocketException& operator=(SlaveSocketException&& other) = delete;

        inline virtual const char* what() const throw() {
            return "Slave-socket error!\n";
        }

    };

}

namespace Sorokin {

    class SlaveSocketConnectionError : public SlaveSocketException {

    public:
        SlaveSocketConnectionError() = default;
        virtual ~SlaveSocketConnectionError() = default;
        SlaveSocketConnectionError(const SlaveSocketConnectionError& other) = default;
        SlaveSocketConnectionError(SlaveSocketConnectionError&& other) = default;

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
        SlaveSocketRecievingError(const SlaveSocketRecievingError& other) = default;
        SlaveSocketRecievingError(SlaveSocketRecievingError&& other) = default;

        SlaveSocketRecievingError& operator=(const SlaveSocketRecievingError& other) = delete;
        SlaveSocketRecievingError& operator=(SlaveSocketRecievingError&& other) = delete;

        inline const char* what() const throw() {
            return "Failed to recieve data!\n";
        }

    };

    class SlaveSocketSendingError : public SlaveSocketException {

    public:
        SlaveSocketSendingError() = default;
        ~SlaveSocketSendingError() = default;
        SlaveSocketSendingError(const SlaveSocketSendingError& other) = default;
        SlaveSocketSendingError(SlaveSocketSendingError&& other) = default;

        SlaveSocketSendingError& operator=(const SlaveSocketSendingError& other) = delete;
        SlaveSocketSendingError& operator=(SlaveSocketSendingError&& other) = delete;

        inline const char* what() const throw() {
            return "Failed to send data!\n";
        }

    };

    class ParserException : public SlaveSocketException {

    public:
        ParserException() = default;
        virtual ~ParserException() = default;
        ParserException(const ParserException& other) = default;
        ParserException(ParserException&& other) = default;

        ParserException& operator=(const ParserException& other) = delete;
        ParserException& operator=(ParserException&& other) = delete;

        inline virtual const char* what() const throw() {
            return "Parser error!\n";
        }

    };

}

namespace Sorokin {

    class EmptyFileError : public ParserException {

    public:
        EmptyFileError() = default;
        ~EmptyFileError() = default;
        EmptyFileError(const EmptyFileError& other) = default;
        EmptyFileError(EmptyFileError&& other) = default;

        EmptyFileError& operator=(const EmptyFileError& other) = delete;
        EmptyFileError& operator=(EmptyFileError&& other) = delete;

        inline const char* what() const throw() {
            return "File is empty!\n";
        }

    };

}

#endif // MASTERSOCKETEXCEPTIONS_H
