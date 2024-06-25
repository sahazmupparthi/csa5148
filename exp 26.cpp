#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#define RSA_KEY_BITS 2048

void print_hex(const char *label, const unsigned char *buf, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; ++i) {
        printf("%02X", buf[i]);
    }
    printf("\n");
}

int main() {
    RSA *rsa = NULL;
    FILE *pub_key_file = fopen("public_key.pem", "r");
    FILE *priv_key_file = fopen("private_key.pem", "r");

    if (!pub_key_file || !priv_key_file) {
        printf("Error: Failed to open key files.\n");
        return 1;
    }

    rsa = PEM_read_RSA_PUBKEY(pub_key_file, NULL, NULL, NULL);
    if (!rsa) {
        printf("Error: Failed to read public key.\n");
        return 1;
    }

    // Example plaintext
    unsigned char plaintext[] = "Hello, RSA!";

    // Buffer for encrypted data
    unsigned char encrypted[RSA_size(rsa)];

    // Encrypt the plaintext
    int encrypted_length = RSA_public_encrypt(strlen((char *)plaintext) + 1, plaintext, encrypted, rsa, RSA_PKCS1_PADDING);
    if (encrypted_length == -1) {
        printf("Error: RSA encryption failed.\n");
        return 1;
    }

    // Print encrypted data
    print_hex("Encrypted", encrypted, encrypted_length);

    // Clean up
    RSA_free(rsa);
    fclose(pub_key_file);
    fclose(priv_key_file);

    return 0;
}
