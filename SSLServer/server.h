/// @file - server object module
#ifndef SERVER_H
#define SERVER_H

#include "headers.h"
#include "exceptions.h"
#include "sockets.h"

namespace Sorokin {

    /**
     * @brief The Server class - implementation of master-socket manager
     */
    class Server {

    public:
        /// @defgroup Server_setup_methods
        /// @{
        /// @brief Default constructor
        Server() noexcept : _masterSocket(nullptr) {}
        /**
         * @brief setSocket - creates socket with certain parametrs
         * @param[in] domain - socket domain. Defines ip-protocol
         * @param[in] type - socket type. Defines transport-protocol
         * @param[in] protocol - protocol type. Defines type of transport-protocol
         * @param[in] ip - server ip
         * @param[in] port - server port
         * @return 0 if no error occured, -1 else
         */
        int setSocket(const int domain        = AF_INET,
                      const int type          = SOCK_STREAM,
                      const int protocol      = 0,
                      const in_addr_t ip      = INADDR_LOOPBACK,
                      const int port          = 12345
                      ) noexcept;
        /// @brief allows to set socket with already created object
        /// @return reference to a pointer to the <Socket> structure
        /// You need to use <deleteSocket> method before if <Socket> object was created previously
        Socket*& accessSocket()& noexcept {
            return _masterSocket;
        }
        /**
         * @brief setUpListener binds socket according to socket info and sets it to listen
         * @return 0 if no error occured, -1 else
         */
        int setUpListener() noexcept;
        /// @}

        /// @defgroup Server_working_methods
        /// @{
        /**
         * @brief getConnection sets up connection with a client
         * @param[out] slaveSocket - descriptor connected to the client
         * @return 0 if no error occured, -1 else
         */
        int getConnection(Socket*& slaveSocket) noexcept(false);
        /// @}

        /// @defgroup Server_closing_methods
        /// @{
        /**
         * @brief closeConnection - closes connection
         * @return 0 if no error occured, -1 else
         */
        int closeConnection() noexcept(false);
        /**
         * @brief deleteSocket - destroys socket descriptor
         * @throw std::exception if unable to destroy descriptor
         */
        void deleteSocket() noexcept(false);
        /// @brief Default destructor
        ~Server() = default;
        /// @}

    private:
        Socket* _masterSocket;

        /**
         * @brief printCError - prints <errno> msg
         * @param preErrorMsg - msg which will be printed before <errno> msg
         */
        void printCError(std::string preErrorMsg) noexcept(false);

        /// @defgroup Server_deleted_methods
        /// @{
        /// Deleted copy constructor
        Server(const Server&) = delete;
        /// Deleted move constructor
        Server(Server&&) = delete;
        /// Deleted copy assigner
        Server& operator =(const Server&) = delete;
        /// Deleted move assigner
        Server& operator =(Server&&) = delete;
        /// @}

    };

    // virtual int sendString(const std::string& strToSend) noexcept(false);
    // virtual int recvString(std::string& strToRecv) noexcept(false);

    /*
    int Manager::sendString(const std::string& strToSend) noexcept(false) {
        uint16_t strLen = static_cast<uint16_t>(strToSend.size());
        int sResult = send(_socket->getSocketfd(), reinterpret_cast<const char*>(&strLen), sizeof(uint16_t), 0);
        if (sResult <= 0) {
            std::cout << "Failed to send data!\n";
            return -1;
        }
        sResult = send(_socket->getSocketfd(), strToSend.c_str(), strToSend.size(), 0);
        if (sResult <= 0) {
            std::cout << "Failed to send data!\n";
            return -1;
        }
        return 0;
    }

    int Manager::recvString(std::string& strToRecv) noexcept(false) {
        uint16_t strLen = 0;
        int rResult = recv(_socket->getSocketfd(), reinterpret_cast<char*>(&strLen), sizeof(uint16_t), 0);
        if (rResult <= 0) {
            std::cout << "Failed to recieve data!\n";
            return -1;
        }

        char* buffer = new char[strLen];
        rResult = recv(_socket->getSocketfd(), buffer, strLen, 0);
        if (rResult <= 0) {
            std::cout << "Failed to recieve data!\n";
            delete[] buffer;
            buffer = nullptr;
            return -1;
        }

        strToRecv.clear();
        for (int i = 0; i < strLen; ++i) {
            strToRecv += buffer[i];
        }

        delete[] buffer;
        buffer = nullptr;
        return 0;
    }
    */

}

#endif // SERVER_H
