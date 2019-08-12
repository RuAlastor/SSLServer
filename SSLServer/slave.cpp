#include "slave.h"

using namespace Sorokin;

//-----------------------------------------------------------------------------------------------------------------------------

Slave::err Slave::sendString( const std::string& strToSend ) const noexcept
{
    uint32_t strLen = static_cast<uint32_t>( strToSend.size() );
    int sResult = send( _slaveSocket->getSocketfd(), reinterpret_cast<const char*>( &strLen ), sizeof( uint32_t ), 0 );
    if ( sResult <= 0 )
    {
        SOCKET_DEBUG_PRINTER( "Failed to send data!" )
        return sendError;
    }
    SOCKET_DEBUG_PRINTER( std::string( "Bytes sent: " ) + std::to_string( sResult ) )
    SOCKET_DEBUG_PRINTER( std::string( "Message:" ) + std::to_string( strLen ) )

    sResult = send( _slaveSocket->getSocketfd(), strToSend.c_str(), strToSend.size(), 0 );
    if ( sResult <= 0 )
    {
        SOCKET_DEBUG_PRINTER( "Failed to send data!" )
        return sendError;
    }
    SOCKET_DEBUG_PRINTER( std::string( "Bytes sent: " ) + std::to_string( sResult ) )
    SOCKET_DEBUG_PRINTER( std::string( "Message: " ) + strToSend )

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Slave::err Slave::recvString(std::string& strToRecv) const noexcept(false)
{
    uint32_t strLen = 0;
    int rResult = recv( _slaveSocket->getSocketfd(), reinterpret_cast<char*>( &strLen ), sizeof( uint32_t ), 0 );
    if ( rResult <= 0 )
    {
        SOCKET_DEBUG_PRINTER( "Failed to recieve data!" )
        return recvError;
    }
    SOCKET_DEBUG_PRINTER( std::string( "Bytes recieved: " ) + std::to_string( rResult ) )
    SOCKET_DEBUG_PRINTER( std::string( "Message: " ) + std::to_string( strLen ) )

    err checker = noError;
    char* buffer = nullptr;
    try
    {
        buffer = new char[strLen];

        rResult = recv( _slaveSocket->getSocketfd(), buffer, strLen, 0 );
        if ( rResult <= 0 )
        {
            SOCKET_DEBUG_PRINTER( "Failed to recieve data!" )
            throw recvError;
        }

        strToRecv.clear();
        for (uint32_t i = 0; i < strLen; ++i) { strToRecv += buffer[i]; }
        SOCKET_DEBUG_PRINTER( std::string( "Bytes recieved: " ) + std::to_string( rResult ) )
        SOCKET_DEBUG_PRINTER( std::string( "Message: " ) + strToRecv )
    }
    catch (const std::bad_alloc& error)
    {
        SOCKET_DEBUG_PRINTER( std::string( "Unable to allocate memory: " ) + error.what() )
        checker = undefinedError;
    }
    catch ( const err& error ) { checker = error; }
    catch (...) { checker = undefinedError; }

    delete[] buffer;
    buffer = nullptr;

    return checker;
}

//-----------------------------------------------------------------------------------------------------------------------------

Slave::err Slave::closeConnection() noexcept
{
    if (_slaveSocket->getSocketfd() == -1)
    {
        SOCKET_DEBUG_PRINTER( "Socket was already closed!" )
        return noError;
    }

    if ( shutdown( _slaveSocket->getSocketfd(), SHUT_RDWR ) != 0 )
    {
        SOCKET_DEBUG_C_PRINTER( "Can't shutdown connection: " )
        return undefinedError;
    }

    return noError;
}

void Slave::deleteSocket() noexcept {
    _slaveSocket->deleteSocketInfo();

    _slaveSocket->closeDescriptor();

    delete _slaveSocket;
    _slaveSocket = nullptr;
}

//-----------------------------------------------------------------------------------------------------------------------------

void Slave::__printCError( const std::string& preErrorMsg ) const noexcept
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
