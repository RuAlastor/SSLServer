#ifndef SLAVE_H
#define SLAVE_H

#include "headers.h"
#include "exceptions.h"
#include "sockets.h"


namespace Sorokin {

    class Slave {

    public:
        enum err {
            undefinedError = -1,
            noError = 0
        };

        Slave() noexcept : _slaveSocket(nullptr) {}
        Socket*& accessSocket()& noexcept(false) {
            return _slaveSocket;
        }

        virtual err sendString(const std::string& strToSend) noexcept(false);
        virtual err recvString(std::string& strToRecv) noexcept(false);

        err closeConnection() noexcept(false);
        void deleteSocket() noexcept(false);
        ~Slave() = default;

    protected:
        Socket* _slaveSocket;

        /**
         * @brief printCError - prints <errno> msg
         * @param preErrorMsg - msg which will be printed before <errno> msg
         */
        void printCError(std::string preErrorMsg) noexcept(false);

    private:
        Slave(const Slave&) = delete;
        Slave(Slave&&) = delete;
        Slave& operator =(const Slave&) = delete;
        Slave& operator =(Slave&&) = delete;
    };

}

#endif // Slave_H
