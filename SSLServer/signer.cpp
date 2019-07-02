#include "signer.h"


using namespace Sorokin;


Signer::Signer(const char *publicLoc, const char *privateLoc) noexcept : _publicLoc(publicLoc), _privateLoc(privateLoc) {}

void Signer::GetAccess() noexcept {
    std::cout << "Enter your password: ";
    std::cin >> _pwd;
}

std::string Signer::SignString(const std::string& stringToSign) noexcept(false) {
    RSA* keys = nullptr;

    FILE *publicFile = nullptr, *privateFile = nullptr;
    publicFile = fopen(_publicLoc, "rb");
    privateFile = fopen(_privateLoc, "rb");
    PEM_read_RSAPublicKey(publicFile, &keys, NULL, NULL);
    PEM_read_RSAPrivateKey(privateFile, &keys, NULL, const_cast<char*>(_pwd.c_str()));
    fclose(publicFile);
    fclose(privateFile);

    // Get signature here

    std::string signStr = "Pretend to be a signature";
    RSA_free(keys);
    return signStr;
}
