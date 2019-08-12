#include "signer.h"


using namespace Sorokin;

//-----------------------------------------------------------------------------------------------------------------------------

std::string Signer::getPubKey() noexcept
{
    std::ifstream pubKeyFile;
    pubKeyFile.open( _publicLoc.c_str(), std::ios_base::binary );
    if ( !pubKeyFile.is_open() )
    {
        SIGNER_DEBUG_PRINTER( "Couldn't open file!" )
        return "";
    }

    std::string strBuf;
    std::getline( pubKeyFile, strBuf );
    std::getline( pubKeyFile, strBuf );
    std::string pubKey = "";
    try
    {
        while( strBuf != "-----END RSA PUBLIC KEY-----" )
        {
            pubKey += strBuf;
            strBuf.clear();
            std::getline( pubKeyFile, strBuf );
        }
    }
    catch ( const std::exception& error )
    {
        SIGNER_DEBUG_PRINTER( std::string( "Unable to read file: " ) + error.what() )
        return "";
    }
    catch (...)
    {
        SIGNER_DEBUG_PRINTER( "Unable to read file: Unknow error!" )
        return "";
    }
    SIGNER_DEBUG_PRINTER( std::string( "Public key:\n" ) + pubKey )

    try { pubKeyFile.close(); }
    catch ( const std::exception& error )
    { SIGNER_DEBUG_PRINTER( std::string( "Unable to close file: " ) + error.what() ) }

    return pubKey;
}

//-----------------------------------------------------------------------------------------------------------------------------

std::string Signer::signString( const std::string& strToSign ) noexcept
{
    RSA* tmpRSA = nullptr;
    if ( __getPrivateKey( &tmpRSA ) )
    {
        SIGNER_DEBUG_PRINTER( "Can't get RSA Keys!" )
        return "";
    }
    SIGNER_DEBUG_PRINTER( "Got private key!" )

    RSA_ptr rsa( tmpRSA, ::RSA_free );
    tmpRSA = nullptr;

    unsigned char* signature = new unsigned char[ RSA_size( rsa.get() ) ];
    unsigned int signatureSize = 0;

    if ( !RSA_sign( NID_sha256,
                    reinterpret_cast<const unsigned char*>( strToSign.c_str() ),
                    strToSign.size(),
                    signature,
                    &signatureSize,
                    rsa.get() ) )
    {
        SIGNER_DEBUG_PRINTER( "Failed to sign:" )
        SIGNER_DEBUG_PRINTER( ERR_error_string( ERR_peek_error(), NULL ) )
    }

    std::string tmp = __turnRawReadable( signature, signatureSize );
    delete[] signature;

    SIGNER_DEBUG_PRINTER( std::string( "Signature:\n" ) + tmp )

    return tmp;
}

//-----------------------------------------------------------------------------------------------------------------------------

Signer::err Signer::getPwd() noexcept(false)
{
    termios oldt;
    tcgetattr( STDIN_FILENO, &oldt );
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );

    while (true)
    {
        std::cout << "Enter your password: ";
        std::cin >> _pwd;
        if (__checkPwd()) {
            break;
        }
        std::cout << "\nWrong password! Try again!\n";
    }
    std::cout << "\nPassword is correct!\n";

    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Signer::err Signer::setKeyLoc( const char* const pubKeyLoc,
                               const char* const privKeyLoc ) noexcept
{
    _publicLoc = pubKeyLoc;
    _privateLoc = privKeyLoc;
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Signer::err Signer::createKeyPair( const unsigned int keyLength ) noexcept(false)
{
    std::string pwd = __createPassword();
    if ( __writePassword(pwd) || __createKeyPairs( pwd, keyLength ) )
    {
        SIGNER_DEBUG_PRINTER( "Failed to create key pair!" )
        return undefinedError;
    }
    SIGNER_DEBUG_PRINTER( "Pair created!" )

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Signer::err Signer::__createKeyPairs( const std::string& pwd,
                                      const unsigned int& keyLength ) noexcept(false)
{
    RSA_ptr rsa( RSA_new(), ::RSA_free );
    BN_ptr bn( BN_new(), ::BN_free );

    if ( rsa.get() == nullptr || bn.get() == nullptr ) { return undefinedError; }

    if (BN_set_word(bn.get(), RSA_F4) != 1)
    {
        SIGNER_DEBUG_PRINTER( "Unable to set exponent:" )
        SIGNER_DEBUG_PRINTER( ERR_error_string( ERR_peek_error(), NULL ) )
        return undefinedError;
    }

    if ( RSA_generate_key_ex( rsa.get(), keyLength, bn.get(), NULL ) != 1 )
    {
        SIGNER_DEBUG_PRINTER( "Unable to create keys: " )
        SIGNER_DEBUG_PRINTER( ERR_error_string( ERR_peek_error(), NULL ) )
        return undefinedError;
    }

    FILE *privKeyFile = nullptr, *pubKeyFile = nullptr;

    privKeyFile = fopen( _privateLoc.c_str(), "wb" );
    pubKeyFile = fopen( _publicLoc.c_str(), "wb" );
    if ( privKeyFile == nullptr || pubKeyFile == nullptr )
    {
        SIGNER_DEBUG_C_PRINTER( "Unable to open file: " )
        return undefinedError;
    }

    err checker = noError;
    try
    {
        const EVP_CIPHER *cipher = nullptr;
        cipher = EVP_get_cipherbyname( "bf-ofb" );
        if ( cipher == nullptr )
        {
            SIGNER_DEBUG_PRINTER( "Unable to find cipher: " )
            SIGNER_DEBUG_PRINTER( ERR_error_string( ERR_peek_error(), NULL ) )
            throw undefinedError;
        }

        if ( PEM_write_RSAPublicKey( pubKeyFile, rsa.get() ) != 1 )
        {
            SIGNER_DEBUG_PRINTER( "Can't write public key: " )
            SIGNER_DEBUG_PRINTER( ERR_error_string( ERR_peek_error(), NULL ) )
            throw undefinedError;
        }

        if ( PEM_write_RSAPrivateKey( privKeyFile, rsa.get(),
                                     cipher,
                                     reinterpret_cast<unsigned char*>( const_cast<char*>( pwd.c_str() ) ), pwd.size(),
                                     NULL,
                                     NULL
                                     ) != 1 )
        {
            SIGNER_DEBUG_PRINTER( "Can't write private key: " )
            SIGNER_DEBUG_PRINTER( ERR_error_string( ERR_peek_error(), NULL ) )
            throw undefinedError;
        }
    }
    catch ( const err& error ) { checker = error; }
    catch (...) { checker = undefinedError; }
    if ( fclose(pubKeyFile)) { SIGNER_DEBUG_C_PRINTER( "Failed to close public key file: " ) }
    if (fclose(privKeyFile)) { SIGNER_DEBUG_C_PRINTER( "Failed to close private key file: " ) }

    return checker;
}

//-----------------------------------------------------------------------------------------------------------------------------

Signer::err Signer::__getPrivateKey( RSA** const keys ) noexcept {
    FILE *privateFile = nullptr;

    privateFile = fopen( _privateLoc.c_str(), "rb" );
    if ( privateFile == nullptr )
    {
        SIGNER_DEBUG_C_PRINTER( "Couldn't open private key file: " )
        return undefinedError;
    }

    OpenSSL_add_all_algorithms();
    PEM_read_RSAPrivateKey( privateFile, keys, NULL, const_cast<char*>( _pwd.c_str() ) );

    if ( fclose(privateFile) ) { SIGNER_DEBUG_C_PRINTER( "Couldn't close private key file: " ) }

    if (keys == nullptr) { return undefinedError; }

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool Signer::__checkPwd() noexcept {
    unsigned int hashLength = 128;
    bool checker = false;
    unsigned char* pwdHash = nullptr;
    try
    {
        pwdHash = new unsigned char[hashLength];
        memset( pwdHash, 0, hashLength );

        if ( MD5( reinterpret_cast<const unsigned char*>( _pwd.c_str() ), _pwd.size(), pwdHash ) == NULL )
        {
            SIGNER_DEBUG_PRINTER( "Unable to hash password!" )
            SIGNER_DEBUG_PRINTER( ERR_error_string( ERR_peek_error(), NULL ) )
            throw false;
        }

        std::string tmp = __turnRawReadable(pwdHash, hashLength);

        int fd = open( "/home/student/C++/SSLServer/pwd.md5", O_RDONLY );
        if ( fd == -1 )
        {
            SIGNER_DEBUG_PRINTER( "Couldn't open the file: " )
            throw false;
        }

        char* tmpBuf = new char[tmp.size()];
        unsigned int rResult = read( fd, tmpBuf, tmp.size() );

        if ( rResult < hashLength )
        {
            SIGNER_DEBUG_PRINTER( "Couldn't read from file: " )
            checker = false;
        }
        if ( close(fd) != 0 ) { SIGNER_DEBUG_C_PRINTER( "Couldn't close the file" ) }

        if ( strncmp( tmp.c_str(), tmpBuf, tmp.size() ) == 0 ) { checker = true; }

        memset( tmpBuf, 0, hashLength );
        delete[] tmpBuf;
    }
    catch (bool& error) { checker = error; }
    catch (...) { checker = undefinedError; }

    delete[] pwdHash;
    return checker;
}

//-----------------------------------------------------------------------------------------------------------------------------

Signer::err Signer::__writePassword( const std::string& pwd ) noexcept {
    unsigned int hashLength = 128;
    err checker = noError;
    unsigned char* pwdHash = nullptr;
    try
    {
        pwdHash = new unsigned char[hashLength];
        memset(pwdHash, 0, hashLength);

        if ( MD5( reinterpret_cast<const unsigned char*>( pwd.c_str() ), pwd.size(), pwdHash ) == NULL )
        {
            SIGNER_DEBUG_PRINTER( "Unable to hash password!" )
            SIGNER_DEBUG_PRINTER( ERR_error_string(ERR_peek_error(), NULL) )
            throw undefinedError;
        }

        std::string tmp = __turnRawReadable( pwdHash, hashLength );

        int fd = open( "/home/student/C++/SSLServer/pwd.md5", O_TRUNC | O_WRONLY );
        if ( fd == -1 )
        {
            SIGNER_DEBUG_C_PRINTER( "Couldn't open the file: " )
            throw undefinedError;
        }

        int wResult = write( fd, tmp.c_str(), tmp.length() );
        if ( wResult < 1 )
        {
            SIGNER_DEBUG_C_PRINTER( "Couldn't write to the file: " )
            throw undefinedError;
        }
        if ( close(fd) != 0 )
        {
            SIGNER_DEBUG_C_PRINTER( "Couldn't close file: " )
            throw undefinedError;
        }
    }
    catch( const std::exception& error )
    {
        SIGNER_DEBUG_PRINTER( error.what() )
        checker = undefinedError;
    }
    catch (err& error) { checker = error; }
    catch (...) { checker = undefinedError; }

    delete[] pwdHash;
    pwdHash = nullptr;
    return checker;
}

//-----------------------------------------------------------------------------------------------------------------------------

std::string Signer::__createPassword() noexcept(false)
{
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

#ifndef SIGNER_PWD_CREATE_DEBUG
    std::string answerStr = "";
    while (true) {
        std::cout << "Enter password for the private key: ";
        std::cin >> answerStr;
        std::cout << "\nRepeat password: ";
        std::string tmp = "";
        std::cin >> tmp;
        if (tmp == answerStr) {
            std::cout << '\n';
            break;
        }
        std::cout << "\nWrong password!\n";
    }
#else
    std::string answerStr = "12345";
#endif

    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return answerStr;
}

//-----------------------------------------------------------------------------------------------------------------------------

std::string Signer::__turnRawReadable( const unsigned char *signature,
                                       const unsigned int& signatureLength ) noexcept(false) {
    std::stringstream ss;
    ss << std::hex;
    for (unsigned int i = 0; i < signatureLength; ++i) {
        ss << std::setw(2) << std::setfill('0') << (int)signature[i];
    }
    return ss.str();
}

//-----------------------------------------------------------------------------------------------------------------------------

void Signer::__printCError( const std::string& preErrorMsg ) const noexcept(false)
{
    error_t errorNum = errno;
    const size_t bufMsgSize = 256;
    char* bufMsg = nullptr;

    try { bufMsg = new char[bufMsgSize]; }
    catch (...)
    {
        SIGNER_DEBUG_PRINTER( "Unable to print C-error!" )
        return;
    }

    bufMsg[0] = '\0';
    char* errorMsg = nullptr;

    errorMsg = strerror_r(  errorNum, bufMsg, bufMsgSize);
    SIGNER_DEBUG_PRINTER( preErrorMsg + errorMsg )

    delete[] bufMsg;
    bufMsg = nullptr;
}

//-----------------------------------------------------------------------------------------------------------------------------
