#include "signer.h"


using namespace Sorokin;


Signer::Signer(const std::string pwd,
               const char *publicLoc,
               const char *privateLoc) noexcept : _pwd(pwd), _publicLoc(publicLoc), _privateLoc(privateLoc) {}

std::string Signer::SignString(const std::string& stringToSign) noexcept(false) {
    RSA* keys = nullptr;
    this->getRSAKeys(&keys);

    unsigned char* signature = new unsigned char[RSA_size(keys)];
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

    std::string tmp = this->turnSignReadable(signature, signatureLength);
    delete[] signature;

    return tmp;
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
    return publicKey;
}

void Signer::getRSAKeys(RSA **keys) noexcept {
    FILE *publicFile = nullptr, *privateFile = nullptr;
    publicFile = fopen(_publicLoc, "rb");
    privateFile = fopen(_privateLoc, "rb");

    OpenSSL_add_all_algorithms();
    PEM_read_RSAPublicKey(publicFile, keys, NULL, NULL);
    PEM_read_RSAPrivateKey(privateFile, keys, NULL, const_cast<char*>(_pwd.c_str()));

    fclose(publicFile);
    fclose(privateFile);
}

std::string Signer::turnSignReadable(const unsigned char *signature, const unsigned int& signatureLength) noexcept(false) {
    std::stringstream ss;
    ss << std::hex;
    for (unsigned int i = 0; i < signatureLength; ++i) {
        ss << std::setw(2) << std::setfill('0') << (int)signature[i];
    }
    return ss.str();
}










