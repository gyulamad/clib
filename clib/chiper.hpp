#pragma once

#include <string>
#include <fstream>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#include "err.hpp"

using namespace std;

namespace clib {

    string base64_encode(const string& input) {
        BIO *bio, *b64;
        BUF_MEM *bptr;

        b64 = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        bio = BIO_new(BIO_s_mem());
        BIO_push(b64, bio);

        BIO_write(b64, input.c_str(), (int)input.length());
        BIO_flush(b64);

        BIO_get_mem_ptr(b64, &bptr);

        string result(bptr->data, bptr->length);

        BIO_free_all(b64);

        return result;
    }

    string base64_decode(const string& input) {
        BIO *bio, *b64;

        bio = BIO_new(BIO_s_mem());
        BIO_write(bio, input.c_str(), (int)input.length());

        b64 = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        BIO_push(b64, bio);

        char buffer[1024];
        int length = BIO_read(b64, buffer, sizeof(buffer));

        BIO_free_all(b64);

        return string(buffer, (size_t)length);
    }

    void rsa_generate(const string& priv_file, const string& pub_file, int bits = 2048) {

        EVP_PKEY* key = EVP_PKEY_new();

        BIGNUM* bn = BN_new();
        BN_set_word(bn, RSA_F4); 

        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
            
        if (EVP_PKEY_keygen_init(ctx) <= 0)
            throw ERROR("EVP_PKEY_keygen_init failed");

        if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) <= 0)
            throw ERROR("EVP_PKEY_CTX_set_rsa_keygen_bits failed");

        if (EVP_PKEY_CTX_set1_rsa_keygen_pubexp(ctx, bn) <= 0)
            throw ERROR("EVP_PKEY_CTX_set1_rsa_keygen_pubexp failed");

        if (EVP_PKEY_keygen(ctx, &key) <= 0)
            throw ERROR("EVP_PKEY_keygen failed");

        FILE* priv_fp = fopen(priv_file.c_str(), "wb");
        if (!priv_fp) throw ERROR("Failed to open: " + priv_file);

        PEM_write_PrivateKey(priv_fp, key, NULL, NULL, 0, NULL, NULL);
        fclose(priv_fp);
        
        FILE* pub_fp = fopen(pub_file.c_str(), "wb");
        if (!pub_fp) throw ERROR("Failed to open: " + pub_file);

        EVP_PKEY* pub_key = EVP_PKEY_new();
        EVP_PKEY_copy_parameters(pub_key, key);
        // EVP_PKEY* pub_key = EVP_PKEY_new();
        // EVP_PKEY_copy_parameters(pub_key, key);
            
        PEM_write_PUBKEY(pub_fp, key);  

        EVP_PKEY_free(key);
        EVP_PKEY_free(pub_key);
        fclose(pub_fp);

        // Cleanup
    }

    string rsa_encrypt(const string& message, const string& public_key_file) {

        FILE* f = fopen(public_key_file.c_str(), "r");
        if (!f) throw ERROR("Failed to open: " + public_key_file);

        EVP_PKEY* key = PEM_read_PUBKEY(f, NULL, NULL, NULL);
        
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(key, NULL);
        
        size_t size = (size_t)EVP_PKEY_size(key) + 1;
        unsigned char* encrypted = (unsigned char*) malloc(size);
        
        EVP_PKEY_encrypt_init(ctx); 
        EVP_PKEY_encrypt(ctx, encrypted, &size, 
            (const unsigned char*) message.c_str(), message.length());
        
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(key);
        fclose(f);

        string ret = string((char*)encrypted, size);
        free(encrypted);
        return ret;
    }


    string rsa_decrypt(const string& encrypted, const string& private_key_file) {

        FILE* f = fopen(private_key_file.c_str(), "r");
        if (!f) throw ERROR("Failed to open: " + private_key_file);

        EVP_PKEY* key = PEM_read_PrivateKey(f, NULL, NULL, NULL);
        
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(key, NULL);
        
        size_t size = (size_t)EVP_PKEY_size(key) + 1;
        unsigned char* decrypted = (unsigned char*) malloc(size);

        EVP_PKEY_decrypt_init(ctx);
        EVP_PKEY_decrypt(ctx, decrypted, &size, 
            (const unsigned char*)encrypted.c_str(), encrypted.length());

        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(key);
        fclose(f);

        string ret = string(reinterpret_cast<char*>(decrypted), size);  
        free(decrypted);
        return ret;
    }

    string encrypt(const string& message, const string& public_key_file) {
        return base64_encode(rsa_encrypt(message, public_key_file));
    }

    string decrypt(const string& message, const string& private_key_file) {
        return rsa_decrypt(base64_decode(message), private_key_file);
    }

}