#include "server.h"

using namespace Sorokin;

//-----------------------------------------------------------------------------------------------------------------------------

Server::err Server::setSocket( const int domain, const int type, const int protocol,
                                                 const in_addr_t ip, const int port ) noexcept
{
    try { _masterSocket = new Socket; }
    catch ( const std::bad_alloc& error )
    {
        SOCKET_DEBUG_PRINTER( std::string( "Unable to allocate memory: " ) + error.what() )
        return bad_alloc;
    }
    catch (...)
    {
        SOCKET_DEBUG_PRINTER( "Unable to allocate memory: Undefined error" )
        return undefinedError;
    }

    if ( _masterSocket->setSocket( domain, type, protocol ) ) return undefinedError;
    if ( _masterSocket->setSocketInfo( domain, ip, port ) )
    {
        _masterSocket->closeDescriptor();
        return undefinedError;
    }

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Server::err Server::setUpListener() noexcept
{
    if ( bind( _masterSocket->getSocketfd(),
               reinterpret_cast<const sockaddr*>( _masterSocket->getSocketInfo() ),
               sizeof( *(_masterSocket->getSocketInfo()) ) ) )
    {
        SOCKET_DEBUG_C_PRINTER( "Couldn't bind the socket: " )
        return bindError;
    }
    SOCKET_DEBUG_PRINTER( "Socket was succesfully binded!" )

    if ( listen( _masterSocket->getSocketfd(), 1 ) )
    {
        SOCKET_DEBUG_C_PRINTER( "Can't set socket to listen: " )
        return listenError;
    }
    SOCKET_DEBUG_PRINTER( "Socket was set to listen successfully!" )

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Server::err Server::getConnection(Socket*& slaveSocket) const noexcept {
    SOCKET_DEBUG_PRINTER( "Waiting for connection..." )

    socklen_t slaveSocketSize = 0;
    try { slaveSocket = new Socket; }
    catch ( const std::bad_alloc& error )
    {
        SOCKET_DEBUG_PRINTER( std::string( "Unable to allocate memory" ) + error.what() )
        return bad_alloc;
    }
    catch (...)
    {
        SOCKET_DEBUG_PRINTER( "Unable to allocate memory: Undefined error!" )
        return undefinedError;
    }

    slaveSocket->accessSocket() = accept( _masterSocket->getSocketfd(),
                                          reinterpret_cast<sockaddr*>( slaveSocket->accessSocketInfo() ),
                                          &slaveSocketSize);
    SOCKET_DEBUG_PRINTER( "Got a connection! " )
    if (slaveSocket->getSocketfd() == -1)
    {
        SOCKET_DEBUG_C_PRINTER( "Failed to accept connection: " )
        return undefinedError;
    }
    SOCKET_DEBUG_PRINTER( "Connection accepted!" )

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Server::err Server::closeConnection() noexcept
{
    if (_masterSocket->getSocketfd() == -1)
    {
        SOCKET_DEBUG_PRINTER( "Listening socket was already closed!" )
        return noError;
    }

    if ( shutdown( _masterSocket->getSocketfd(), SHUT_RDWR ) != 0 )
    {
        SOCKET_DEBUG_C_PRINTER( "Can't shutdown connection: " )
        return undefinedError;
    }

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Server::deleteSocket() noexcept
{
    _masterSocket->deleteSocketInfo();

    _masterSocket->closeDescriptor();

    delete _masterSocket;
    _masterSocket = nullptr;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Server::__printCError(const std::string& preErrorMsg) const noexcept
{
    error_t errorNum = errno;
    const size_t bufMsgSize = 256;
    char* bufMsg = nullptr;

    try { bufMsg = new char[bufMsgSize]; }
    catch (...)
    {
        SOCKET_DEBUG_PRINTER( "Unable to print C-error!" )
        return;
    }

    bufMsg[0] = '\0';
    char* errorMsg = nullptr;

    errorMsg = strerror_r(  errorNum, bufMsg, bufMsgSize);
    SOCKET_DEBUG_PRINTER( preErrorMsg + errorMsg )

    delete[] bufMsg;
    bufMsg = nullptr;
}

//-----------------------------------------------------------------------------------------------------------------------------
