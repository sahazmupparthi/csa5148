#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define AES_BLOCK_SIZE 16

// Function to perform AES encryption in CBC mode for CBC-MAC
int aes_cbc_mac(unsigned char *key, unsigned char *input, int input_len, unsigned char *mac) {
    EVP_CIPHER_CTX *ctx;

    if (!(ctx = EVP_CIPHER_CTX_new())) {
        return 0;
    }

    // Initialize the context for encryption with AES-128 CBC
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, NULL)) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    // Encrypt the input data
    int len;
    if (1 != EVP_EncryptUpdate(ctx, mac, &len, input, input_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    // Finalize the encryption (if any padding is needed)
    if (1 != EVP_EncryptFinal_ex(ctx, mac + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    EVP_CIPHER_CTX_free(ctx);
    return 1;
}

int main() {
    unsigned char key[AES_BLOCK_SIZE] = "0123456789ABCDEF";  // Example key (128 bits)
    unsigned char X[AES_BLOCK_SIZE] = "This is a message";  // Example message (one block)

    unsigned char mac[AES_BLOCK_SIZE];  // MAC output buffer

    // Calculate CBC-MAC for the message X
    if (!aes_cbc_mac(key, X, AES_BLOCK_SIZE, mac)) {
        fprintf(stderr, "Error: AES CBC-MAC calculation failed.\n");
        return 1;
    }

    // Print the MAC (CBC-MAC for X)
    printf("CBC-MAC for X: ");
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%02X", mac[i]);
    }
    printf("\n");

    // Calculate CBC-MAC for the two-block message X || (X XOR T)
    unsigned char X_concat[AES_BLOCK_SIZE * 2];
    memcpy(X_concat, X, AES_BLOCK_SIZE);  // X
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        X_concat[AES_BLOCK_SIZE + i] = X[i] ^ mac[i];  // X XOR T
    }

    unsigned char mac_two_blocks[AES_BLOCK_SIZE];
    if (!aes_cbc_mac(key, X_concat, AES_BLOCK_SIZE * 2, mac_two_blocks)) {
        fprintf(stderr, "Error: AES CBC-MAC calculation failed for two blocks.\n");
        return 1;
    }

    // Print the MAC for X || (X XOR T)
    printf("CBC-MAC for X || (X XOR T): ");
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%02X", mac_two_blocks[i]);
    }
    printf("\n");

    return 0;
}
