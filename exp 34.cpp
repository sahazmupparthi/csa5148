#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

#define AES_BLOCK_SIZE 16

// Function to print bytes as hexadecimal
void print_hex(const unsigned char *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02X", buf[i]);
    }
    printf("\n");
}

// Function to pad plaintext according to PKCS7 padding scheme
int pad_plaintext(const unsigned char *plaintext, size_t plaintext_len,
                  unsigned char **padded_plaintext, size_t *padded_len) {
    size_t padding_len = AES_BLOCK_SIZE - (plaintext_len % AES_BLOCK_SIZE);
    *padded_len = plaintext_len + padding_len;
    
    *padded_plaintext = (unsigned char *)malloc(*padded_len);
    if (!(*padded_plaintext)) {
        perror("malloc");
        return 0;
    }
    
    memcpy(*padded_plaintext, plaintext, plaintext_len);
    memset(*padded_plaintext + plaintext_len, padding_len, padding_len);
    
    return 1;
}

// Function to remove PKCS7 padding from plaintext
int unpad_plaintext(const unsigned char *padded_plaintext, size_t padded_len,
                    unsigned char **plaintext, size_t *plaintext_len) {
    if (padded_len % AES_BLOCK_SIZE != 0) {
        fprintf(stderr, "Invalid padded plaintext length.\n");
        return 0;
    }
    
    size_t padding_len = padded_plaintext[padded_len - 1];
    *plaintext_len = padded_len - padding_len;
    
    *plaintext = (unsigned char *)malloc(*plaintext_len);
    if (!(*plaintext)) {
        perror("malloc");
        return 0;
    }
    
    memcpy(*plaintext, padded_plaintext, *plaintext_len);
    
    return 1;
}

// Function to encrypt plaintext using AES in ECB mode
int aes_ecb_encrypt(const unsigned char *plaintext, size_t plaintext_len,
                    unsigned char *ciphertext, const unsigned char *key) {
    AES_KEY aes_key;
    
    if (AES_set_encrypt_key(key, 128, &aes_key) != 0) {
        fprintf(stderr, "AES_set_encrypt_key failed.\n");
        return 0;
    }
    
    AES_encrypt(plaintext, ciphertext, &aes_key);
    
    return 1;
}

// Function to decrypt ciphertext using AES in ECB mode
int aes_ecb_decrypt(const unsigned char *ciphertext, size_t ciphertext_len,
                    unsigned char *plaintext, const unsigned char *key) {
    AES_KEY aes_key;
    
    if (AES_set_decrypt_key(key, 128, &aes_key) != 0) {
        fprintf(stderr, "AES_set_decrypt_key failed.\n");
        return 0;
    }
    
    AES_decrypt(ciphertext, plaintext, &aes_key);
    
    return 1;
}

// Function to encrypt plaintext using AES in CBC mode
int aes_cbc_encrypt(const unsigned char *plaintext, size_t plaintext_len,
                    unsigned char *ciphertext, const unsigned char *key,
                    const unsigned char *iv) {
    AES_KEY aes_key;
    AES_set_encrypt_key(key, 128, &aes_key);
    
    AES_cbc_encrypt(plaintext, ciphertext, plaintext_len, &aes_key, iv, AES_ENCRYPT);
    
    return 1;
}

// Function to decrypt ciphertext using AES in CBC mode
int aes_cbc_decrypt(const unsigned char *ciphertext, size_t ciphertext_len,
                    unsigned char *plaintext, const unsigned char *key,
                    const unsigned char *iv) {
    AES_KEY aes_key;
    AES_set_decrypt_key(key, 128, &aes_key);
    
    AES_cbc_encrypt(ciphertext, plaintext, ciphertext_len, &aes_key, iv, AES_DECRYPT);
    
    return 1;
}

// Function to encrypt plaintext using AES in CFB mode
int aes_cfb_encrypt(const unsigned char *plaintext, size_t plaintext_len,
                    unsigned char *ciphertext, const unsigned char *key,
                    const unsigned char *iv, int *num) {
    AES_KEY aes_key;
    AES_set_encrypt_key(key, 128, &aes_key);
    
    AES_cfb128_encrypt(plaintext, ciphertext, plaintext_len, &aes_key, iv, num, AES_ENCRYPT);
    
    return 1;
}

// Function to decrypt ciphertext using AES in CFB mode
int aes_cfb_decrypt(const unsigned char *ciphertext, size_t ciphertext_len,
                    unsigned char *plaintext, const unsigned char *key,
                    const unsigned char *iv, int *num) {
    AES_KEY aes_key;
    AES_set_encrypt_key(key, 128, &aes_key);
    
    AES_cfb128_encrypt(ciphertext, plaintext, ciphertext_len, &aes_key, iv, num, AES_DECRYPT);
    
    return 1;
}

int main() {
    // Example key and IV (Initialization Vector)
    unsigned char key[AES_BLOCK_SIZE] = { 0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
                                          0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };
    unsigned char iv[AES_BLOCK_SIZE] = { 0x3D, 0xAE, 0x9E, 0xC9, 0x78, 0x2F, 0x57, 0xA6,
                                         0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };
    
    // Example plaintext
    unsigned char plaintext[] = "Hello, AES! This is a message for testing ECB, CBC, and CFB modes.";
    size_t plaintext_len = strlen((const char *)plaintext);
    
    // Pad the plaintext (PKCS7 padding)
    unsigned char *padded_plaintext = NULL;
    size_t padded_len = 0;
    if (!pad_plaintext(plaintext, plaintext_len, &padded_plaintext, &padded_len)) {
        fprintf(stderr, "Error padding plaintext.\n");
        return 1;
    }
    
    // Allocate memory for ciphertext
    unsigned char ciphertext[padded_len];
    
    // Encrypt using ECB mode
    aes_ecb_encrypt(padded_plaintext, padded_len, ciphertext, key);
    printf("ECB Mode:\n");
    printf("Ciphertext: ");
    print_hex(ciphertext, padded_len);
    
    // Decrypt using ECB mode
    unsigned char decrypted_ecb[padded_len];
    aes_ecb_decrypt(ciphertext, padded_len, decrypted_ecb, key);
    printf("Decrypted plaintext: %s\n\n", decrypted_ecb);
    
    // Encrypt using CBC mode
    aes_cbc_encrypt(padded_plaintext, padded_len, ciphertext, key, iv);
    printf("CBC Mode:\n");
    printf("Ciphertext: ");
    print_hex(ciphertext, padded_len);
    
    // Decrypt using CBC mode
    unsigned char decrypted_cbc[padded_len];
    aes_cbc_decrypt(ciphertext, padded_len, decrypted_cbc, key, iv);
    printf("Decrypted plaintext: %s\n\n", decrypted_cbc);
    
    // Encrypt using CFB mode
    int num = 0; // Initialize number of bits used in the IV
    aes_cfb_encrypt(padded_plaintext, padded_len, ciphertext, key, iv, &num);
    printf("CFB Mode:\n");
    printf("Ciphertext: ");
    print_hex(ciphertext, padded_len);
    
    // Decrypt using CFB mode
    unsigned char decrypted_cfb[padded_len];
    aes_cfb_decrypt(ciphertext, padded_len, decrypted_cfb, key, iv, &num);
    printf("Decrypted plaintext: %s\n", decrypted_cfb);
    
    // Clean up
    free(padded_plaintext);
    
    return 0;
}
