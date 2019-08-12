/// @file Socket.h - socket object module
#ifndef SOCKET_H
#define SOCKET_H

// General libs
#include <string>
#include <cstring>                      /// Needed to transform error into readable state

#include <unistd.h>                     /// Needed for close()
#include <fcntl.h>                      /// Needed to make sockets non-block
#include <errno.h>                      /// Needed to check errors in C-functions

// Server libs
#include <sys/socket.h>                 /// Main posix-socket library
#include <sys/types.h>                  /// Main posix-socket library
#include <netinet/in.h>                 /// Needed for proper ip and port reading

#define VARIABLES
#define METHODS

#define SOCKET_DEBUG
//#undef SOCKET_DEBUG

#ifdef SOCKET_DEBUG
    #include <iostream>
    #define SOCKET_DEBUG_PRINTER( msg ) std::cout << msg << '\n';
    #define SOCKET_DEBUG_C_PRINTER( preErrorMsg ) __printCError( preErrorMsg );
#else
    #define SOCKET_DEBUG_PRINTER(msg)
    #define SOCKET_DEBUG_C_PRINTER( preErrorMsg )
#endif

namespace Sorokin {

//-----------------------------------------------------------------------------------------------------------------------------

     /**
     * @brief The Socket class - object implementation of posix-sockets
     */
    class Socket
    {

    public VARIABLES:
        /// @brief Return error types
        enum err
        {
            closingRepetition   = -3,
            bad_alloc           = -2,
            undefinedError      = -1,
            noError             = 0
        };

    public METHODS:
        /// @defgroup SOCKET_SETUP
        /// @{
        /// @brief Socket - default constructor
        Socket() noexcept : _socketfd( -1 ), _socketInfo( nullptr ) {}
        /**
         * @brief accessSocket - returns reference to a socket to set
         * @return reference to a socket descriptor variable
         *
         * You need to use <closeDescriptor> method before if previously you've created socket var
         */
        inline int& accessSocket()& noexcept { return _socketfd; }
        /**
         * @brief accessSocketInfo - returns a reference to the pointer to the structure for setting
         * @return reference to a pointer to a <sockaddr_in> structure, which points to a nullptr
         *
         * You need to use <deleteSocketInfo> method before if previously you've already set it
         */
        inline sockaddr_in*& accessSocketInfo()& noexcept { return _socketInfo; }
        /**
         * @brief setSocket - creates an instance of <socket> structure
         * @param[in] domain - socket domain. Defines ip-protocol
         * @param[in] type - socket type. Defines transport-protocol
         * @param[in] protocol - protocol type. Defines type of transport-protocol
         * @return 0 if no error occured, -1 otherwise
         */
        err setSocket(const int domain, const int type, const int protocol) noexcept;
        /**
         * @brief setSocketInfo - allocates memory for <sockaddr_in> structure
         * @param[in] domain - socket domain. Defines ip-protocol
         * @param[in] ip - server ip
         * @param[in] port - server port
         * @return 0 if no error occured, -1 otherwise
         */
        err setSocketInfo(const int domain, const in_addr_t ip, const int port) noexcept;
        /// @}

        /// @defgroup SOCKET_GETTERS
        /// @{
        /// @brief gives a copy of the socket file descriptor
        inline int getSocketfd() const noexcept { return _socketfd; }
        /// @brief gives a reference to a constant <sockaddr_in> structure
        inline const sockaddr_in* getSocketInfo() const noexcept { return _socketInfo; }
        /// @}

        /// @defgroup SOCKET_DELETERS
        /// @{
        /// @brief deletes <sockaddr_in> structure
        void deleteSocketInfo() noexcept;
        /// @brief destroys descriptor
        err closeDescriptor() noexcept;
        /// @brief ~Socket - default destructor
        virtual ~Socket() noexcept = default;
        /// @}

    protected VARIABLES:
        int _socketfd;
        sockaddr_in* _socketInfo;

    protected METHODS:
        /// @defgroup SOCKET_ADDITIONAL
        /// @{
        /**
         * @brief setNonBlock - sets current socket in a non-block state
         * @return 0 if no error occured, -1 else
         */
        int __setNonBlock() noexcept;
        /**
         * @brief printCError - prints <errno> msg
         * @param preErrorMsg - msg which will be printed before <errno> msg
         */
        void __printCError(const std::string& preErrorMsg) const noexcept;
        /// @}

    private METHODS:
        /// @defgroup SOCKET_DELETED
        /// @{
        /// @brief Deleted copy constructor
        Socket(const Socket&)               = delete;
        /// @brief Deleted move constructor
        Socket(Socket&&)                    = delete;
        /// @brief Deleted copy assigner
        Socket& operator =(const Socket&)   = delete;
        /// @brief Deleted move assigner
        Socket& operator =(Socket&&)        = delete;
        /// @}

    };
//-----------------------------------------------------------------------------------------------------------------------------

}

#endif // SOCKET_H
