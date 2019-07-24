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
        enum err {
            undefinedError = -1,
            noError = 0
        };

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
        err setSocket(const int domain        = AF_INET,
                      const int type          = SOCK_STREAM,
                      const int protocol      = 0,
                      const in_addr_t ip      = INADDR_LOOPBACK,
                      const int port          = 12345
                      ) noexcept;
        /// @brief allows to set socket with already created object
        /// @return reference to a pointer to the <Socket> structure
        /// You need to use <deleteSocket> method before if <Socket> object was created previously
        inline Socket*& accessSocket()& noexcept { return _masterSocket; }
        /**
         * @brief setUpListener binds socket according to socket info and sets it to listen
         * @return 0 if no error occured, -1 else
         */
        err setUpListener() noexcept;
        /// @}

        /// @defgroup Server_working_methods
        /// @{
        /**
         * @brief getConnection sets up connection with a client
         * @param[out] slaveSocket - descriptor connected to the client
         * @return 0 if no error occured, -1 else
         */
        err getConnection(Socket*& slaveSocket) noexcept(false);
        /// @}

        /// @defgroup Server_closing_methods
        /// @{
        /**
         * @brief closeConnection - closes connection
         * @return 0 if no error occured, -1 else
         */
        err closeConnection() noexcept(false);
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

}

#endif // SERVER_H
