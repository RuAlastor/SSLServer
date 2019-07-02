#ifndef SIGNER_H
#define SIGNER_H

#include <headers.h>

namespace Sorokin {

    class Signer {

    public:

        explicit Signer(const char* publicLoc, const char* privateLoc) noexcept;
        ~Signer() noexcept = default;

        Signer() = delete;
        Signer(const Signer& other) = delete;
        Signer(Signer&& other) = delete;
        Signer& operator=(const Signer& other) = delete;
        Signer& operator=(Signer&& other) = delete;

        void GetAccess() noexcept;
        unsigned char* SignString(const std::string& stringToSign) noexcept(false);
        std::string GetPublicKey() noexcept(false);

    private:
        const char* _publicLoc;
        const char* _privateLoc;
        std::string _pwd;


    };

}

#endif // SIGNER_H
