#ifndef SIGNER_H
#define SIGNER_H

#include "headers.h"

namespace Sorokin {

//-----------------------------------------------------------------------------------------------------------------------------


    class Signer {

    public:
        enum err {
            unknownError = -1,
            noError = 0
        };

    public:
        Signer() = default;
        ~Signer() = default;

        err createKeyPair() noexcept(false);

    private:
        std::string _pwd;
        std::string _publicLoc;  // "/home/student/C++/public_key"
        std::string _privateLoc; // "/home/student/C++/private_key"

    private:
        std::string __createPassword() noexcept(false);
        std::string __turnRawReadable(const unsigned char* signature,
                                      const unsigned int& signatureLength
                                      ) noexcept(false);
        err __writePassword(const std::string& pwd) noexcept(false);
        err __createKeyPairs(const std::string& pwd) noexcept(false);

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
