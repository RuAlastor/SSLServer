#include "signer.h"


using namespace Sorokin;


Signer::Signer(const char *publicLoc, const char *privateLoc) noexcept : _publicLoc(publicLoc), _privateLoc(privateLoc) {}

void Signer::GetAccess() noexcept {
    // std::cout << "Enter your password: ";
    // std::cin >> _pwd;
    _pwd = "12345";
}

unsigned char* Signer::SignString(const std::string& stringToSign) noexcept(false) {
    RSA* keys = nullptr;

    FILE *publicFile = nullptr, *privateFile = nullptr;
    publicFile = fopen(_publicLoc, "rb");
    privateFile = fopen(_privateLoc, "rb");

    PEM_read_RSAPublicKey(publicFile, &keys, NULL, NULL);
    PEM_read_RSAPrivateKey(privateFile, &keys, NULL, const_cast<char*>(_pwd.c_str()));

    fclose(publicFile);
    fclose(privateFile);

    unsigned char* signature = new unsigned char[RSA_size(keys)]; // (unsigned char*)malloc(RSA_size(keys));
    unsigned int signatureLength;

    if (!RSA_sign(NID_sha256,
                  reinterpret_cast<const unsigned char*>(stringToSign.c_str()),
                  stringToSign.size(),
                  signature,
                  &signatureLength,
                  keys)) {
        std::cout << "Failed to sign!\n";
        std::cout << ERR_error_string(ERR_peek_error(), NULL) << std::endl;
    }

    RSA_free(keys);

    return signature;
}

std::string Signer::GetPublicKey() noexcept(false) {
    std::ifstream publicKeyFile;
    publicKeyFile.open(_publicLoc);
    std::string publicKey = "";
    std::string strBuffer;
    std::getline(publicKeyFile, strBuffer);
    while (!publicKeyFile.eof()) {
        publicKey += strBuffer;
        strBuffer.clear();
        std::getline(publicKeyFile, strBuffer);
    }
    std::cout << publicKey << std::endl;
    return publicKey;
}













