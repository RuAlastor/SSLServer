/// @file - server object module
#ifndef SERVER_H
#define SERVER_H

#include "sockets.h"

namespace Sorokin {

//-----------------------------------------------------------------------------------------------------------------------------

    /**
     * @brief The Server class - implementation of master-socket manager
     */
    class Server
    {

    public VARIABLES:
        /// @brief Return error types
        enum err
        {
            listenError     = -4,
            bindError       = -3,
            bad_alloc       = -2,
            undefinedError  = -1,
            noError         = 0
        };

    public METHODS:
        /// @defgroup SERVER_SETTERS
        /// @{
        /// @brief Default constructor
        Server() noexcept : _masterSocket(nullptr) {}
        /// @brief allows to set socket with already created object
        /// @return reference to a pointer to the <Socket> structure
        /// You need to use <deleteSocket> method before if <Socket> object was created previously
        inline Socket*& accessSocket()& noexcept { return _masterSocket; }
        /**
         * @brief setSocket - creates socket with certain parametrs
         * @param[in] domain - socket domain. Defines ip-protocol
         * @param[in] type - socket type. Defines transport-protocol
         * @param[in] protocol - protocol type. Defines type of transport-protocol
         * @param[in] ip - server ip
         * @param[in] port - server port
         * @return 0 if no error occured, -1 else
         */
        err setSocket(const int domain, const int type, const int protocol,
                                        const in_addr_t ip, const int port ) noexcept;
        /**
         * @brief setUpListener binds socket according to socket info and sets it to listen
         * @return 0 if no error occured, -1 else
         */
        err setUpListener() noexcept;
        /// @}

        /// @defgroup SERVER_MAIN
        /// @{
        /**
         * @brief getConnection sets up connection with a client
         * @param[out] slaveSocket - descriptor connected to the client
         * @return 0 if no error occured, -1 else
         */
        err getConnection(Socket*& slaveSocket) const noexcept;
        /// @}

        /// @defgroup Server_closing_methods
        /// @{
        /**
         * @brief closeConnection - closes connection
         * @return 0 if no error occured, -1 else
         */
        err closeConnection() noexcept;
        /**
         * @brief deleteSocket - destroys socket descriptor
         * @throw std::exception if unable to destroy descriptor
         */
        void deleteSocket() noexcept;
        /// @brief Default destructor
        ~Server() = default;
        /// @}

    private VARIABLES:
        Socket* _masterSocket;

    private METHODS:
        /// @defgroup SERVER_ADDITIONAL
        /// @{
        /**
         * @brief printCError - prints <errno> msg
         * @param preErrorMsg - msg which will be printed before <errno> msg
         */
        void __printCError(const std::string& preErrorMsg) const noexcept;
        /// @}

    private METHODS:
        /// @defgroup SERVER_DELETED
        /// @{
        /// Deleted copy constructor
        Server(const Server&)               = delete;
        /// Deleted move constructor
        Server(Server&&)                    = delete;
        /// Deleted copy assigner
        Server& operator =(const Server&)   = delete;
        /// Deleted move assigner
        Server& operator =(Server&&)        = delete;
        /// @}

    };

//-----------------------------------------------------------------------------------------------------------------------------

}

#endif // SERVER_H
