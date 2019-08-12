#ifndef SIGNER_H
#define SIGNER_H

// General libs
#include <iostream>
#include <string>
#include <sstream>                      /// Needed to properly turn raw data into hex
#include <iomanip>                      /// Needed to properly turn raw data into hex
#include <memory>                       /// Needed to properly work with OpenSSL ptr's
#include <fstream>                      /// Needed to properly read public key
#include <cstring>                      /// Needed to transform error into readable state

#include <errno.h>                      /// Needed to check errors in C-functions
#include <unistd.h>                     /// Needed for close()
#include <fcntl.h>                      /// Needed to make sockets non-block
#include <stdio.h>                      /// Needed to use RSA functions
#include <termios.h>                    /// Needed to hide pwd

// Security part
#include <openssl/rsa.h>                /// Main OpenSSL libraries
#include <openssl/md5.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bn.h>

#define SIGNER_DEBUG
// #undef SIGNER_DEBUG

#define SIGNER_PWD_CREATE_DEBUG
#undef SIGNER_PWD_CREATE_DEBUG

#ifdef SIGNER_DEBUG
    #define SIGNER_DEBUG_PRINTER( msg ) std::cout << msg << '\n';
    #define SIGNER_DEBUG_C_PRINTER( preErrorMsg ) __printCError( preErrorMsg );
#else
    #define SIGNER_DEBUG_PRINTER(msg)
    #define SIGNER_DEBUG_C_PRINTER(preErrorMsg)
#endif

namespace Sorokin {

//-----------------------------------------------------------------------------------------------------------------------------

    class Signer {

    public:
        enum err
        {
            undefinedError = -1,
            noError = 0
        };

        using RSA_ptr   = std::unique_ptr<RSA, decltype(&::RSA_free)>;
        using BN_ptr    = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;

    public:
        Signer() = default;
        err createKeyPair(const unsigned int keyLength) noexcept(false);
        err setKeyLoc(const char* const pubKeyLoc, const char* const privKeyLoc) noexcept;

        err getPwd() noexcept(false);
        std::string signString(const std::string& strToSign) noexcept;
        std::string getPubKey() noexcept;

        ~Signer() = default;

    private:
        std::string _pwd;
        std::string _publicLoc;
        std::string _privateLoc;

    private:
        err __getPrivateKey(RSA** const key) noexcept;
        std::string __createPassword() noexcept(false);
        std::string __turnRawReadable(const unsigned char* signature,
                                      const unsigned int& signatureLength
                                      ) noexcept(false);
        err __writePassword(const std::string& pwd) noexcept;
        err __createKeyPairs(const std::string& pwd, const unsigned int& keyLength) noexcept(false);
        bool __checkPwd() noexcept;

    private:
        /**
         * @brief printCError - prints <errno> msg
         * @param preErrorMsg - msg which will be printed before <errno> msg
         */
        void __printCError(const std::string& preErrorMsg) const noexcept(false);

    private:
        Signer(const Signer&) = delete;
        Signer(Signer&&) = delete;
        Signer& operator = (const Signer&) = delete;
        Signer& operator = (Signer&&) = delete;

    };

//-----------------------------------------------------------------------------------------------------------------------------

}

#endif // SIGNER_H
