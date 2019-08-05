/// @file Socket.h - socket object module
#ifndef SOCKET_H
#define SOCKET_H

// General libs
#include <iostream>

#include <unistd.h>                     /// Needed for close()
#include <fcntl.h>                      /// Needed to make sockets non-block
#include <errno.h>                      // Needed to check errors in C-functions
#include <string.h>                     // Needed to transform error into readable state

// Server libs
#include <sys/socket.h>                 /// Main posix-socket library
#include <sys/types.h>                  /// Main posix-socket library
#include <netinet/in.h>                 /// Needed for proper ip and port reading

#ifdef DEBUG
    #define SOCKET_DEBUG
    //#undef SOCKET_DEBUG
#endif

namespace Sorokin {

//-----------------------------------------------------------------------------------------------------------------------------

    /// @brief Exception type class
    class SocketError : public std::exception {

    public:
        /// @defgroup Socket_except_setup_methods
        /// @{
        /// @brief Default constructor
        SocketError() noexcept : _errorMsg(nullptr) {}
        /**
         * @brief SocketError - constructor with an argument
         * @param[in] inputMsg - error message
         */
        SocketError(const char* inputMsg) noexcept {
            _errorMsg = new char[strlen(inputMsg)];
            strcpy(_errorMsg, inputMsg);
        }
        /// @brief Default copy constructor
        SocketError(const SocketError& other) noexcept {
            _errorMsg = new char[strlen(other._errorMsg)];
            strcpy(this->_errorMsg, other._errorMsg);
        }
        /// @brief Default move constructor
        SocketError(SocketError&& other) {
            this->_errorMsg = other._errorMsg;
            other._errorMsg = nullptr;
        }
        /// @}

        /// @defgroup Socket_except_delete_methods
        /// @{
        /// @brief Default destructor
        virtual ~SocketError() noexcept {
            delete[] _errorMsg;
        }
        /// @}

        /// @defgroup Socket_except_data_using_methods
        /// @{
        /// @brief what() - returns error message
        virtual const char* what() const noexcept {
            if (_errorMsg != nullptr) {
                return _errorMsg;
            }
            else {
                return "Unknown socket error!";
            }
        }
        /// @}

    private:
        char* _errorMsg;

    private:
        /// @defgroup Socket_except_deleted_methods
        /// @{
        /// @brief Deleted copy assigner
        SocketError& operator =(const SocketError&) = delete;
        /// @brief Deleted move assigner
        SocketError& operator =(SocketError&&) = delete;
        /// @}

    };

//-----------------------------------------------------------------------------------------------------------------------------

     /**
     * @brief The Socket class - object implementation of posix-sockets
     */
    class Socket {

    public:
        /// @brief Return error types
        enum err {
            undefinedError = -1,
            noError = 0
        };

    public:
        /// @defgroup Socket_setup_methods
        /// @{
        /// @brief Socket - default constructor
        Socket() noexcept;
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
        err setSocket(const int domain     = AF_INET,
                      const int type       = SOCK_STREAM,
                      const int protocol   = 0
                      ) noexcept;
        /**
         * @brief setSocketInfo - allocates memory for <sockaddr_in> structure
         * @param[in] domain - socket domain. Defines ip-protocol
         * @param[in] ip - server ip
         * @param[in] port - server port
         * @return 0 if no error occured, -1 otherwise
         */
        err setSocketInfo(const int domain      = AF_INET,
                          const in_addr_t ip    = INADDR_LOOPBACK,
                          const int port        = 12345
                          ) noexcept;
        /// @}

        /// @defgroup Socket_info_using_methods
        /// @{
        /// @brief gives a copy of the socket file descriptor
        inline int getSocketfd() const noexcept { return _socketfd; }
        /// @brief gives a reference to a constant <sockaddr_in> structure
        inline const sockaddr_in* getSocketInfo() const noexcept { return _socketInfo; }
        /// @}

        /// @defgroup Socket_closing_methods
        /// @{
        /// @brief deletes <sockaddr_in> structure
        void deleteSocketInfo() noexcept(false);
        /// @brief destroys descriptor
        err closeDescriptor() noexcept(false);
        /// @brief ~Socket - default destructor
        virtual ~Socket() noexcept = default;
        /// @}

    protected:
        int _socketfd;
        sockaddr_in* _socketInfo;

    protected:
        /// @defgroup Socket_additional_methods
        /// @{
        /**
         * @brief setNonBlock - sets current socket in a non-block state
         * @return 0 if no error occured, -1 else
         */
        int __setNonBlock() noexcept;
        /// @brief throws <errno> msg
        void __throwCError() noexcept(false);
        /**
         * @brief printCError - prints <errno> msg
         * @param preErrorMsg - msg which will be printed before <errno> msg
         */
        void __printCError(std::string preErrorMsg) noexcept(false);
        /// @}

    private:
        /// @defgroup Socket_deleted_methods
        /// @{
        /// @brief Deleted copy constructor
        Socket(const Socket&) = delete;
        /// @brief Deleted move constructor
        Socket(Socket&&) = delete;
        /// @brief Deleted copy assigner
        Socket& operator =(const Socket&) = delete;
        /// @brief Deleted move assigner
        Socket& operator =(Socket&&) = delete;
        /// @}

    };
//-----------------------------------------------------------------------------------------------------------------------------

}

#endif // SOCKET_H
