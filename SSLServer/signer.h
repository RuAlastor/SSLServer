#ifndef SIGNER_H
#define SIGNER_H

// General libs
#include <iostream>
#include <sstream>                      /// Needed to properly turn raw data into hex
#include <iomanip>                      // Needed to properly turn raw data into hex
#include <memory>                       /// Needed to properly work with OpenSSL ptr's
#include <fstream>                      /// Needed to properly read public key
#include <string>

#include <errno.h>                      /// Needed to check errors in C-functions
#include <string.h>                     /// Needed to transform error into readable state
#include <unistd.h>                     /// Needed for close()
#include <fcntl.h>                      /// Needed to make sockets non-block
#include <stdio.h>                      /// Needed to use RSA functions

// Security part
#include <openssl/rsa.h>                /// Main OpenSSL libraries
#include <openssl/md5.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bn.h>

#ifdef DEBUG
    #define SIGNER_DEBUG
    // #undef SIGNER_DEBUG
#endif

namespace Sorokin {

//-----------------------------------------------------------------------------------------------------------------------------

    class Signer {

    public:
        enum err {
            undefinedError = -1,
            noError = 0
        };

        using RSA_ptr   = std::unique_ptr<RSA, decltype(&::RSA_free)>;
        using BN_ptr    = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;

    public:
        Signer() = default;
        err createKeyPair(const unsigned int keyLength) noexcept(false);
        err setKeyLoc(const char* pubKeyLoc, const char* privKeyLoc) noexcept;

        err getPwd() noexcept(false);
        std::string signString(const std::string& strToSign) noexcept(false);
        std::string getPubKey() noexcept(false);

        ~Signer() = default;

    private:
        std::string _pwd;
        std::string _publicLoc;
        std::string _privateLoc;

    private:
        err __getPrivateKey(RSA** key) noexcept;
        std::string __createPassword() noexcept(false);
        std::string __turnRawReadable(const unsigned char* signature,
                                      const unsigned int& signatureLength
                                      ) noexcept(false);
        err __writePassword(const std::string& pwd) noexcept(false);
        err __createKeyPairs(const std::string& pwd, const unsigned int& keyLength) noexcept(false);
        bool __checkPwd() noexcept(false);

    private:
        /**
         * @brief printCError - prints <errno> msg
         * @param preErrorMsg - msg which will be printed before <errno> msg
         */
        void __printCError(std::string preErrorMsg) noexcept(false);

    private:
        Signer(const Signer&) = delete;
        Signer(Signer&&) = delete;
        Signer& operator = (const Signer&) = delete;
        Signer& operator = (Signer&&) = delete;

    };

//-----------------------------------------------------------------------------------------------------------------------------

}

#endif // SIGNER_H
