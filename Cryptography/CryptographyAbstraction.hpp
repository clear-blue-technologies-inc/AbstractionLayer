/***************************************************************************//**
* @author   Ben Haubrich
* @file     CryptographyAbstraction.hpp
* @details  Abstraction layer for Cryptography
* @ingroup  AbstractionLayer
*******************************************************************************/
#ifndef __CBT_CRYPTOGRAPHY_ABSTRACTION_HPP__
#define __CBT_CRYPTOGRAPHY_ABSTRACTION_HPP__

//Foundation
#include "CbtError.hpp"
#include "CbtTypes.hpp"
//C++
#include <string>

/**
 * @enum CryptographyAlgorithm
 * @brief Cryptography algorithms for generating keys
*/
enum class CryptographyAlgorithm {
    EllipticCurveDiffieHellman, ///< Elliptic Curve Diffie-Hellman
    Curve25519,                 ///< Curve25519
    AeadChaCha20Poly1305Ietf    ///< AEAD ChaCha20Poly1305 IETF

};

/**
 * @class CryptographyAbstraction
 * @brief Abstraction layer for Cryptography
*/
class CryptographyAbstraction {

    public:
    /**
     * @brief Constructor
     * @param[in] keySize The size of the keys to generate
     * @param[in] privateStaticKey Initialize the private and public static keys with an existing private key.
     * @post If privateStaticKey is not empty, the private and public static keys will be initialized with the private key.
     *       If privateStaticKey is empty, the private and public static keys will be generated.
    */
    CryptographyAbstraction(const std::string &privateStaticKey, Bytes keySize) : _privateKey(privateStaticKey) {
        _privateKey.reserve(keySize);
        _publicKey.reserve(keySize);
    }
    virtual ~CryptographyAbstraction() = default;

    /**
     * @brief Generate a static private and public key for encryption/decryption
     * @returns ErrorType::Success if the key was generated successfully
    */
    virtual ErrorType generateKeys(CryptographyAlgorithm algorithm) = 0;
    
    /**
     * @brief Generate a new secret key
     * @pre All input keys have been sized appropriately (i.e std::string::resize())
     * @param[in] algorithm The algorithm to generate the key
     * @param[in] myPrivateKey The local private key to use.
     * @param[in] theirPublicKey The remote public key to use.
     * @param[out] newPrivateKey The generated secret key
     * @returns ErrorType::Success if the key was generated successfully
     * @post The size of the new private key can be obtained from privateKey.size()
    */
    virtual ErrorType generatePrivateKey(CryptographyAlgorithm algorithm, const std::string &myPrivateKey, const std::string &theirPublicKey, std::string &newPrivateKey) = 0;

    /**
     * @brief Encrypt data
     * @param[in] algorithm The algorithm to use for encryption
     * @param[in] dataToEncrypt The data to encrypt
     * @param[out] encryptedData The encrypted data
     * @return ErrorType::Success if the data was encrypted successfully
     */
    virtual ErrorType encrypt(CryptographyAlgorithm algorithm, const std::string &dataToEncrypt, std::string &encryptedData, ...) = 0;

    /**
     * @brief Decrypt data
     * @param[in] algorithm The algorithm to use for decryption
     * @param[in] encryptedData The data to decrypt
     * @param[out] decryptedData The decrypted data
     * @returns ErrorType::Success if the data was decrypted successfully
     * @returns ErrorType::Failure if the data was not decrypted successfully.
     */
    virtual ErrorType decrypt(CryptographyAlgorithm algorithm, const std::string &encryptedData, std::string &decryptedData, ...) = 0;

    /// @brief Get the private key as a constant reference
    const std::string &privateKeyConst() const { return _privateKey; }
    /// @brief Get the public key as a constant reference
    const std::string &publicKeyConst() const { return _publicKey; }
    /// @brief Get the private key as a mutable reference
    std::string &privateKey() { return _privateKey; }
    /// @brief Get the public key as a mutable reference
    std::string &publicKey() { return _publicKey; }

    protected:
    /// @brief The private key
    std::string _privateKey;
    /// @brief The public key
    std::string _publicKey;
};

#endif // __CBT_CRYPTOGRAPHY_ABSTRACTION_HPP__
