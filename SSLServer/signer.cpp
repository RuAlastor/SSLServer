#include "signer.h"


using namespace Sorokin;

//-----------------------------------------------------------------------------------------------------------------------------

std::string Signer::getPubKey() noexcept(false) {
    std::ifstream pubKeyFile;
    pubKeyFile.open(_publicLoc.c_str(), std::ios_base::binary);
    if (!pubKeyFile.is_open()) {
        std::cout << "Couldn't open file!\n";
        return "";
    }

    std::string strBuf;
    std::getline(pubKeyFile, strBuf);
    std::getline(pubKeyFile, strBuf);
    std::string pubKey = "";
    while(strBuf != "-----END RSA PUBLIC KEY-----") {
        pubKey += strBuf;
        strBuf.clear();
        std::getline(pubKeyFile, strBuf);
    }

    pubKeyFile.close();

#ifdef SIGNER_DEBUG
    std::cout << "Public key:\n"
              << pubKey << '\n';
#endif

    return pubKey;
}

//-----------------------------------------------------------------------------------------------------------------------------

std::string Signer::signString(const std::string& strToSign) noexcept(false) {
    RSA* tmpRSA = nullptr;
    if (__getPrivateKey(&tmpRSA)) {
        std::cout << "Can't get RSA Keys!\n";
        return "";
    }
    std::cout << "Got private key!\n";

    RSA_ptr rsa(tmpRSA, ::RSA_free);
    tmpRSA = nullptr;

    unsigned char* signature = new unsigned char[RSA_size(rsa.get())];
    unsigned int signatureSize = 0;

    if (!RSA_sign(NID_sha256,
                  reinterpret_cast<const unsigned char*>(strToSign.c_str()),
                  strToSign.size(),
                  signature,
                  &signatureSize,
                  rsa.get())) {
        std::cout << "Failed to sign!\n";
        std::cout << ERR_error_string(ERR_peek_error(), NULL) << std::endl;
    }

    std::string tmp = __turnRawReadable(signature, signatureSize);
    delete[] signature;

#ifdef SIGNER_DEBUG
    std::cout << "Signature:\n"
              << tmp << '\n';
#endif

    return tmp;
}

//-----------------------------------------------------------------------------------------------------------------------------

Signer::err Signer::getPwd() noexcept(false) {
    while (true) {
        std::cout << "Enter your password: ";
        std::cin >> _pwd;
        if (__checkPwd()) {
            break;
        }
        std::cout << "Wrong password! Try again!\n";
    }
    std::cout << "Password is correct!\n";
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Signer::err Signer::setKeyLoc(const char* pubKeyLoc, const char* privKeyLoc) noexcept{
    _publicLoc = pubKeyLoc;
    _privateLoc = privKeyLoc;
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Signer::err Signer::createKeyPair(const unsigned int keyLength) noexcept(false) {
    std::string pwd = __createPassword();
    if (__writePassword(pwd) || __createKeyPairs(pwd, keyLength)) {
        std::cout << "Failed to create key pair!\n";
        return undefinedError;
    }
    std::cout << "Pair created!\n";
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Signer::err Signer::__createKeyPairs(const std::string& pwd, const unsigned int& keyLength) noexcept(false) {
    RSA_ptr rsa(RSA_new(), ::RSA_free);
    BN_ptr bn(BN_new(), ::BN_free);

    if (rsa.get() == nullptr || bn.get() == nullptr) {
        return undefinedError;
    }
    if (BN_set_word(bn.get(), RSA_F4) != 1) {
        std::cout << "Unable to set exponent: ";
        std::cout << ERR_error_string(ERR_peek_error(), NULL) << std::endl;
        return undefinedError;
    }

    if (RSA_generate_key_ex(rsa.get(), keyLength, bn.get(), NULL) != 1) {
        std::cout << "Unable to create keys: ";
        std::cout << ERR_error_string(ERR_peek_error(), NULL) << std::endl;
        return undefinedError;
    }

    FILE *privKeyFile = nullptr, *pubKeyFile = nullptr;

    privKeyFile = fopen(_privateLoc.c_str(), "wb");
    pubKeyFile = fopen(_publicLoc.c_str(), "wb");
    if (privKeyFile == nullptr || pubKeyFile == nullptr) {
        __printCError("Couldn't find files: ");
        return undefinedError;
    }

    err checker = noError;
    try {
        const EVP_CIPHER *cipher = nullptr;
        cipher = EVP_get_cipherbyname("bf-ofb");
        if (cipher == nullptr) {
            std::cout << "Unable to find cipher: ";
            std::cout << ERR_error_string(ERR_peek_error(), NULL) << std::endl;
            throw undefinedError;
        }

        if (PEM_write_RSAPublicKey(pubKeyFile, rsa.get()) != 1) {
            std::cout << "Can't write public key: ";
            std::cout << ERR_error_string(ERR_peek_error(), NULL) << std::endl;
            throw undefinedError;
        }

        if (PEM_write_RSAPrivateKey(privKeyFile, rsa.get(),
                                    cipher,
                                    reinterpret_cast<unsigned char*>(const_cast<char*>(pwd.c_str())), pwd.size(),
                                    NULL,
                                    NULL
                                    ) != 1) {
            std::cout << "Can't write private key: ";
            std::cout << ERR_error_string(ERR_peek_error(), NULL) << std::endl;
            throw undefinedError;
        }
    }
    catch (err& error) {
        checker = error;
    }
    if (fclose(pubKeyFile)) {
        __printCError("Failed to close public key file: ");
    }
    if (fclose(privKeyFile)) {
        __printCError("Failed to close private key file: ");
    }
    return checker;
}

//-----------------------------------------------------------------------------------------------------------------------------

Signer::err Signer::__getPrivateKey(RSA** keys) noexcept {
    FILE *privateFile = nullptr;
    privateFile = fopen(_privateLoc.c_str(), "rb");
    if (privateFile == nullptr) {
        __printCError("Couldn't open private key file: ");
        return undefinedError;
    }

    OpenSSL_add_all_algorithms();
    PEM_read_RSAPrivateKey(privateFile, keys, NULL, const_cast<char*>(_pwd.c_str()));

    if (fclose(privateFile)) {
        __printCError("Couldn't close private key file: ");
    }

    if (keys == nullptr) {
        return undefinedError;
    }
    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

bool Signer::__checkPwd() noexcept(false) {
    unsigned int hashLength = 128;
    bool checker = false;
    unsigned char* pwdHash = nullptr;
    try {
        pwdHash = new unsigned char[hashLength];
        memset(pwdHash, 0, hashLength);

        if (MD5(reinterpret_cast<const unsigned char*>(_pwd.c_str()), _pwd.size(), pwdHash) == NULL) {
            std::cout << "Unable to hash password!\n";
            std::cout << ERR_error_string(ERR_peek_error(), NULL) << std::endl;
            throw false;
        }

        std::string tmp = __turnRawReadable(pwdHash, hashLength);

        int fd = open("/home/student/C++/SSLServer/pwd.md5", O_RDONLY);
        if (fd == -1) {
            __printCError("Couldn't open the file: ");
            throw false;
        }

        char* tmpBuf = new char[tmp.size()];
        unsigned int rResult = read(fd, tmpBuf, tmp.size());
        if (rResult < hashLength) {
            __printCError("Couldn't read from file: ");
            throw false;
        }
        if (close(fd) != 0) {
            __printCError("Couldn't close the file");
        }
        if (strncmp(tmp.c_str(), tmpBuf, tmp.size()) == 0) {
            checker = true;
        }
        memset(tmpBuf, 0, hashLength);
        delete[] tmpBuf;
    }
    catch (bool& error) {
        checker = error;
    }

    delete[] pwdHash;
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
            throw undefinedError;
        }

        std::string tmp = __turnRawReadable(pwdHash, hashLength);

        int fd = open("/home/student/C++/SSLServer/pwd.md5", O_TRUNC | O_WRONLY);
        if (fd == -1) {
            __printCError("Couldn't open the file: ");
            throw undefinedError;
        }

        int wResult = write(fd, tmp.c_str(), tmp.length());
        if (wResult < 1) {
            __printCError("Couldn't write to the file: ");
            throw undefinedError;
        }
        if (close(fd) != 0) {
            __printCError("Couldn't close file: ");
            throw undefinedError;
        }
    }
    catch(std::exception& error) {
        std::cout << error.what() << '\n';
        checker = undefinedError;
    }
    catch (err& error) {
        checker = error;
    }

    delete[] pwdHash;
    pwdHash = nullptr;
    return checker;
}

//-----------------------------------------------------------------------------------------------------------------------------

std::string Signer::__createPassword() noexcept(false) {
#ifndef SIGNER_DEBUG
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

void Signer::__printCError(std::string preErrorMsg) noexcept(false) {
    error_t errorNum = errno;
    const size_t bufMsgSize = 256;
    char* bufMsg = new char[bufMsgSize];
    bufMsg[0] = '\0';
    char* errorMsg = nullptr;

    errorMsg = strerror_r(errorNum, bufMsg, bufMsgSize);
    std::cout << preErrorMsg << errorMsg << '\n';

    delete[] bufMsg;
    bufMsg = nullptr;
}

//-----------------------------------------------------------------------------------------------------------------------------
