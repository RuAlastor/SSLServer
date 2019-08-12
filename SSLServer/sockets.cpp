#include "sockets.h"

using namespace Sorokin;

//-----------------------------------------------------------------------------------------------------------------------------

Socket::err Socket::setSocket( const int domain, const int type, const int protocol ) noexcept
{
    _socketfd = socket( domain, type, protocol );
    if ( _socketfd == -1 )
    {
        SOCKET_DEBUG_C_PRINTER( "Failed to create socket: " )
        return undefinedError;
    }
    SOCKET_DEBUG_PRINTER( "Socket created!" )
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Socket::err Socket::setSocketInfo( const int domain, const in_addr_t ip,  const int port ) noexcept
{
    try { _socketInfo = new sockaddr_in; }
    catch ( const std::bad_alloc& error )
    {
        SOCKET_DEBUG_PRINTER( std::string( "Failed to set socket info" ) + error.what() )
        return bad_alloc;
    }
    catch (...)
    {
        SOCKET_DEBUG_PRINTER( "Failed to set socket info: Undefined error!" )
        return undefinedError;
    }

    memset( _socketInfo, 0, sizeof( *_socketInfo ) );

    _socketInfo->sin_family = domain;
    _socketInfo->sin_port = htons( port );
    _socketInfo->sin_addr.s_addr = htonl( ip );

    SOCKET_DEBUG_PRINTER( "Socket info was set succesfully!" )

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------


void Socket::deleteSocketInfo() noexcept
{
    delete _socketInfo;
    _socketInfo = nullptr;

    SOCKET_DEBUG_PRINTER( "Socket info was successfully deleted!" )
}

//-----------------------------------------------------------------------------------------------------------------------------

Socket::err Socket::closeDescriptor() noexcept {
    if ( _socketfd == -1 )
    {
        SOCKET_DEBUG_PRINTER( "Socket is already closed!" )
        return closingRepetition;
    }

    if ( close(_socketfd) != 0 ) {
        SOCKET_DEBUG_C_PRINTER(  "Failed to close socket: "  )
        return undefinedError;
    }
    SOCKET_DEBUG_PRINTER( "Descriptor was successfully closed!" )

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

int Socket::__setNonBlock() noexcept {
    int flags;
#ifdef O_NONBLOCK
    if ( ( flags = fcntl( _socketfd, F_GETFL, 0 ) ) == -1 ) { flags = 0; }
    return fcntl( _socketfd, F_SETFL, flags | O_NONBLOCK );
#else
    flags = 1;
    return ioctl( _socketfd, FIOBIO, &flags );
#endif
}

//-----------------------------------------------------------------------------------------------------------------------------

void Socket::__printCError( const std::string& preErrorMsg ) const noexcept
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

