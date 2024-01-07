#pragma once

#include <cassert>

#include "../../clib/chiper.hpp"

using namespace std;
using namespace clib;

class ChiperTests {
public:

    static void test_base64() {        
        // Test data
        string originalData = "Hello, OpenSSL Base64!";

        // Base64 encoding
        string encodedData = base64_encode(originalData);

        // Base64 decoding
        string decodedData = base64_decode(encodedData);

        // Test assertions
        assert(decodedData == originalData);
        assert(encodedData != originalData);  // Encoded data should be different from the original
    }

    static void test_rsa() {
        remove("test_public_key.pem");
        remove("test_private_key.pem");

        const char* public_key_file = "test_public_key.pem";
        const char* private_key_file = "test_private_key.pem";

        // Generate RSA key pair
        rsa_generate(private_key_file, public_key_file, 2048);

        // Test data
        string originalMessage = "Hello, RSA!";

        // Encrypt the message using the public key
        string encryptedMessage = rsa_encrypt(originalMessage, public_key_file);

        // Decrypt the message using the private key
        string decryptedMessage = rsa_decrypt(encryptedMessage, private_key_file);

        // Test assertions
        assert(decryptedMessage == originalMessage);
        assert(encryptedMessage != originalMessage);  // Encrypted data should be different from the original

        remove("test_public_key.pem");
        remove("test_private_key.pem");
    }
};