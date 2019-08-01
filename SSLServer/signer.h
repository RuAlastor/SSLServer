#ifndef SIGNER_H
#define SIGNER_H

#include "headers.h"

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


/*
    class Signer {

    public:

        explicit Signer(const std::string pwd,
                        const char* publicLoc = "/home/student/C++/public_key",
                        const char* privateLoc = "/home/student/C++/private_key") noexcept;
        ~Signer() noexcept = default;

        Signer() = delete;
        Signer(const Signer& other) = delete;
        Signer(Signer&& other) = delete;
        Signer& operator=(const Signer& other) = delete;
        Signer& operator=(Signer&& other) = delete;

        std::string SignString(const std::string& stringToSign) noexcept(false);
        std::string GetPublicKey() noexcept(false);

    private:
        const std::string _pwd;
        const char* _publicLoc;
        const char* _privateLoc;

        void getRSAKeys(RSA** keys) noexcept;
        std::string turnSignReadable(const unsigned char* signature, const unsigned int& signatureLength) noexcept(false);

    };
*/

}

#endif // SIGNER_H
