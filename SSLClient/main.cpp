#include "client.h"

int verify(const std::string& strToVerify,
           const std::string& signature,
           const std::string& sigPubKey);

int main() {
    Sorokin::Client client("/home/student/C++/unsignedXML.xml",
                           "/home/student/C++/signedXML.xml",
                           12345);
    try {
        client.Start();
        client.Handle();
    }
    catch (std::exception& error) {
        std::cout << error.what() << '\n';
        return -1;
    }

    std::list<std::string> signatures;
    Sorokin::Parser parser("/home/student/C++/signedXML.xml", &signatures);
    try {
        parser.loadDocument();
        parser.parseDocument();
        parser.Clear();
    }
    catch (std::exception& error) {
        std::cout << error.what() << '\n';
        return -1;
    }

    for (auto iter = signatures.begin(); iter != signatures.end(); ++iter) {
        std::string strToVerify = *iter;
        iter++;
        std::string signature = *iter;
        iter++;
        std::string sigPubKey = *iter;
        if (verify(strToVerify, signature, sigPubKey)) {
            std::cout << "Verified!\n";
        }
        else {
            std::cout << "Wrong!\n";
        }
    }

    return 0;
}

int verify(const std::string &strToVerify,
           const std::string &signature,
           const std::string &sigPubKey) {
    std::cout << strToVerify << '\n';

    // Turn signature into bytes array
    unsigned int ucharSignatureLength = signature.size() / 2;
    unsigned char* ucharSignature = new unsigned char[ucharSignatureLength];
    for (size_t i = 0, j = 0; i < ucharSignatureLength; j+=2, ++i) {
        ucharSignature[i] = (signature[j] % 32 + 9) % 25 * 16 + (signature[j + 1] % 32 + 9) % 25;
    }

    // Put pubkey into RSA_struct
    RSA* publicKey = nullptr;

    std::ofstream fout;
    fout.open("/home/student/C++/public_key_client", std::ios_base::trunc | std::ios_base::binary);
    for (unsigned int i = 0; i < sigPubKey.size(); ++i) {
        fout << sigPubKey[i];
        if (i == 29 || i == 217) {
            fout << '\n';
        }
    }
    fout << '\n';
    fout.close();


    FILE* publicKeyLoc = nullptr;
    publicKeyLoc = fopen("/home/student/C++/public_key_client", "rb");
    PEM_read_RSAPublicKey(publicKeyLoc, &publicKey, NULL, NULL);
    fclose(publicKeyLoc);


    // Verify
    int tmp = RSA_verify(NID_sha256,
                         reinterpret_cast<const unsigned char*>(strToVerify.c_str()),
                         strToVerify.size(),
                         ucharSignature,
                         ucharSignatureLength,
                         publicKey);

    delete[] ucharSignature;
    RSA_free(publicKey);

    return tmp;
}
