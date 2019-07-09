#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "headers.h"

namespace Sorokin {

    class ClientException : public std::exception {

    public:
        ClientException() = default;
        virtual ~ClientException() = default;

        ClientException(const ClientException& other) = default;
        ClientException(ClientException&& other) = default;
        ClientException& operator=(const ClientException& other) = default;
        ClientException& operator =(ClientException&& other) = default;

        inline virtual const char* what() const throw() {
            return "Client exception!";
        }

    };

    class ClientInitError : public ClientException {

    public:
        ClientInitError() = default;
        ~ClientInitError() = default;

        ClientInitError(const ClientInitError& other) = default;
        ClientInitError(ClientInitError&& other) = default;
        ClientInitError& operator=(const ClientInitError& other) = default;
        ClientInitError& operator =(ClientInitError&& other) = default;

        inline const char* what() const throw() {
            return "Failed to initialize socket!";
        }

    };

    class ClientConnectError : public ClientException {

    public:
        ClientConnectError() = default;
        ~ClientConnectError() = default;

        ClientConnectError(const ClientConnectError& other) = default;
        ClientConnectError(ClientConnectError&& other) = default;
        ClientConnectError& operator =(const ClientConnectError& other) = default;
        ClientConnectError& operator =(ClientConnectError&& other) = default;

        inline const char* what() const throw() {
            return "Failed to connect to server!";
        }

    };

    class ClientSendError : public ClientException {

    public:
        ClientSendError() = default;
        ~ClientSendError() = default;

        ClientSendError(const ClientSendError& other) = default;
        ClientSendError(ClientSendError&& other) = default;
        ClientSendError& operator =(const ClientSendError& other) = default;
        ClientSendError& operator =(ClientSendError&& other) = default;

        inline const char* what() const throw() {
            return "Failed to send data!";
        }

    };

    class ClientRecvError : public ClientException {

    public:
        ClientRecvError() = default;
        ~ClientRecvError() = default;

        ClientRecvError(const ClientRecvError& other) = default;
        ClientRecvError(ClientRecvError&& other) = default;
        ClientRecvError& operator =(const ClientRecvError& other) = default;
        ClientRecvError& operator =(ClientRecvError&& other) = default;

        inline const char* what() const throw() {
            return "Failed to recieve data!";
        }
    };

    class FileError : public std::exception {

    public:
        FileError() = default;
        ~FileError() = default;

        FileError(const FileError& other) = default;
        FileError(FileError&& other) = default;
        FileError& operator =(const FileError& other) = default;
        FileError& operator =(FileError&& other) = default;

        inline const char* what() const throw() {
            return "File error!";
        }
    };

}


#endif // EXCEPTIONS_H













