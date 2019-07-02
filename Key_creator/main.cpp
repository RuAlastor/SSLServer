#include <iostream>
#include <string>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/rand.h>



int main(int argc, char* argv[]) {

    // Default password for private key
    std::string pwd = "12345";
    if (argc > 1) {
        pwd = argv[1];
    }

    // Create RSA keys
    RSA* rsa = nullptr;
    const unsigned int keyLength = 256;
    const unsigned long exponentPrime = RSA_F4;
    rsa = RSA_generate_key(keyLength, exponentPrime, NULL, NULL);
    if (rsa == NULL) {
        std::cout << "Failed to create keys!\n";
        return -1;
    }

    // Set files to write keys to
    FILE *private_file = nullptr, *public_file = nullptr;
    private_file = fopen("/home/student/C++/private_key", "wb");
    public_file = fopen("/home/student/C++/public_key", "wb");

    // Set cipher algorithm
    const EVP_CIPHER *cipher = nullptr;
    cipher = EVP_get_cipherbyname("bf-ofb");

    // Write keys to files
    PEM_write_RSAPrivateKey(private_file, rsa, cipher, NULL, 0, NULL, const_cast<char*>(pwd.c_str()));
    PEM_write_RSAPublicKey(public_file, rsa);

    // Clear memory
    RSA_free(rsa);
    fclose(private_file);
    fclose(public_file);

    std::cout << "Keys were written to the file\n";

    return 0;
}
