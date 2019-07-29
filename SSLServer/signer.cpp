#include "signer.h"


using namespace Sorokin;

//-----------------------------------------------------------------------------------------------------------------------------

Signer::err Signer::createKeyPair() noexcept(false) {
    std::string pwd = __createPassword();
    if (__writePassword(pwd) || __createKeyPairs(pwd)) {
        std::cout << "Fail!\n";
        return unknownError;
    }
    std::cout << "Pair created!\n";
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Signer::err Signer::__createKeyPairs(const std::string& pwd) noexcept(false) {
    RSA* rsa = nullptr;
    const unsigned int keyLength = 1024;
    const unsigned long exponentPrime = RSA_F4;

    rsa = RSA_generate_key(keyLength, exponentPrime, NULL, NULL);
    if (rsa == NULL) {
        std::cout << "Unable to create keys: ";
        std::cout << ERR_error_string(ERR_peek_error(), NULL) << std::endl;
        return unknownError;
    }
    err checker = noError;
    try {
        FILE *privKeyFile = nullptr, *pubKeyFile = nullptr;

        privKeyFile = fopen("/home/student/C++/SSLServer/priv_key.key", "wb");
        pubKeyFile = fopen("/home/student/C++/SSLServer/pub_key.key", "wb");
        if (privKeyFile == nullptr || pubKeyFile == nullptr) {
            int errNum = errno;
            std::cout << "Unable to open files: " << strerror(errNum) << '\n';
            throw unknownError;
        }

        const EVP_CIPHER *cipher = nullptr;
        cipher = EVP_get_cipherbyname("bf-ofb");
        if (cipher == nullptr) {
            std::cout << "Unable to find cipher: ";
            std::cout << ERR_error_string(ERR_peek_error(), NULL) << std::endl;
            throw unknownError;
        }

        if (PEM_write_RSAPublicKey(pubKeyFile, rsa) != 1) {
            std::cout << "Can't write public key: ";
            std::cout << ERR_error_string(ERR_peek_error(), NULL) << std::endl;
            throw unknownError;
        }

        if (PEM_write_RSAPrivateKey(privKeyFile, rsa,
                                cipher,
                                reinterpret_cast<unsigned char*>(const_cast<char*>(pwd.c_str())), pwd.size(),
                                NULL,
                                NULL
                                ) != 1) {
            std::cout << "Can't write private key: ";
            std::cout << ERR_error_string(ERR_peek_error(), NULL) << std::endl;
            throw unknownError;
        }
    }
    catch (err& error) {
        checker = error;
    }
    RSA_free(rsa);
    return checker;
}

//-----------------------------------------------------------------------------------------------------------------------------

Signer::err Signer::__writePassword(const std::string& pwd) noexcept(false) {
    unsigned int hashLength = 128;
    err checker = noError;
    unsigned char* pwdHash = nullptr;
    try {
        pwdHash = new unsigned char[hashLength];
        memset(pwdHash, 0, hashLength);

        if (MD5(reinterpret_cast<const unsigned char*>(pwd.c_str()), pwd.size(), pwdHash) == NULL) {
            std::cout << "Unable to hash password!\n";
            std::cout << ERR_error_string(ERR_peek_error(), NULL) << std::endl;
            throw unknownError;
        }

        std::string tmp = __turnRawReadable(pwdHash, hashLength);
        delete[] pwdHash;
        pwdHash = nullptr;

        int errNum = 0;
        int fd = open("/home/student/C++/SSLServer/pwd.md5", O_TRUNC | O_WRONLY);
        if (fd == -1) {
            errNum = errno;
            std::cout << "Unable to open file: " << strerror(errNum) << '\n';
            throw unknownError;
        }

        int wResult = write(fd, tmp.c_str(), tmp.length());
        if (wResult < 1) {
            int errNum = errno;
            std::cout << "Couldn't write in file: " << strerror(errNum) << '\n';
            throw unknownError;
        }
        if (close(fd) != 0) {
            int errNum = errno;
            std::cout << "Couldn't close file: " << strerror(errNum) << '\n';
            throw unknownError;
        }
    }
    catch(std::exception& error) {
        std::cout << error.what() << '\n';
        checker = unknownError;
    }
    catch (err& error) {
        checker = error;
    }

    return checker;
}

//-----------------------------------------------------------------------------------------------------------------------------

std::string Signer::__createPassword() noexcept(false) {
#ifndef DEBUG
    std::string answerStr = "";
    while (true) {
        std::cout << "Enter password for the private key: ";
        std::cin >> answerStr;
        std::cout << "Repeat password: ";
        std::string tmp = "";
        std::cin >> tmp;
        if (tmp == answerStr) {
            break;
        }
        std::cout << "Wrong password!\n";
    }
#else
    std::string answerStr = "12345";
#endif
    return answerStr;
}

//-----------------------------------------------------------------------------------------------------------------------------

std::string Signer::__turnRawReadable(const unsigned char *signature, const unsigned int& signatureLength) noexcept(false) {
    std::stringstream ss;
    ss << std::hex;
    for (unsigned int i = 0; i < signatureLength; ++i) {
        ss << std::setw(2) << std::setfill('0') << (int)signature[i];
    }
    return ss.str();
}

//-----------------------------------------------------------------------------------------------------------------------------


/*
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


*/








