//Modules
#include "CryptographyModule.hpp"
//C++
#include <cstring>

Cryptography::Cryptography(const std::string &privateStaticKey, Bytes keySize) : CryptographyAbstraction(privateStaticKey, keySize) {
}

ErrorType Cryptography::generateKeys(CryptographyAlgorithm alogorithm) {

    switch (alogorithm) {
        case CryptographyAlgorithm::Curve25519:
        case CryptographyAlgorithm::EllipticCurveDiffieHellman:
        case CryptographyAlgorithm::AeadChaCha20Poly1305Ietf:
        default:
            return ErrorType::NotImplemented;
    }
}

ErrorType Cryptography::generatePrivateKey(CryptographyAlgorithm algorithm, const std::string &myPrivateKey, const std::string &theirPublicKey, std::string &newPrivateKey) {
    return ErrorType::NotImplemented;
}

ErrorType Cryptography::encrypt(CryptographyAlgorithm algorithm, const std::string &dataToEncrypt, std::string &encryptedData, ...) {
    return ErrorType::NotImplemented;
}

ErrorType Cryptography::decrypt(CryptographyAlgorithm algorithm, const std::string &encrpytedData, std::string &decryptedData, ...) {
    return ErrorType::NotImplemented;
}

ErrorType Cryptography::generateKeysX25519() {
    return ErrorType::NotImplemented;
}

ErrorType Cryptography::generateKeysEllipticCurveDiffieHellman() {
    return ErrorType::NotImplemented;
}