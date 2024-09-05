#ifndef __CRYPTOGRAPHY_MODULE_HPP__
#define __CRYPTOGRAPHY_MODULE_HPP__

//Foundation
#include "CryptographyAbstraction.hpp"

class Cryptography : public CryptographyAbstraction {
    
    public:
    Cryptography(const std::string &privateStaticKey, Bytes keySize);
    virtual ~Cryptography() = default;

    ErrorType generateKeys(CryptographyAlgorithm algorithm) override;
    ErrorType generatePrivateKey(CryptographyAlgorithm algorithm, const std::string &myPrivateKey, const std::string &theirPublicKey, std::string &newPrivateKey) override;
    ErrorType encrypt(CryptographyAlgorithm algorithm, const std::string &dataToEncrypt, std::string &encryptedData, ...) override;
    ErrorType decrypt(CryptographyAlgorithm algorithm, const std::string &encrpytedData, std::string &decryptedData, ...) override;

    private:
    ErrorType generateKeysX25519();
    ErrorType generateKeysEllipticCurveDiffieHellman();
};

#endif // __CRYPTOGRAPHY_MODULE_HPP__