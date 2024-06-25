#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>

// Function to print bytes as hexadecimal
void print_hex(const unsigned char *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02X", buf[i]);
    }
    printf("\n");
}

// Function to perform DES encryption
int des_encrypt(const unsigned char *plaintext, int plaintext_len,
                unsigned char *ciphertext, const unsigned char *key) {
    DES_key_schedule key_schedule;
    DES_set_key_unchecked((const_DES_cblock *)key, &key_schedule);
    
    DES_ecb_encrypt((const_DES_cblock *)plaintext, (DES_cblock *)ciphertext, &key_schedule, DES_ENCRYPT);
    
    return 1;
}

// Function to perform DES decryption
int des_decrypt(const unsigned char *ciphertext, int ciphertext_len,
                unsigned char *plaintext, const unsigned char *key) {
    DES_key_schedule key_schedule;
    DES_set_key_unchecked((const_DES_cblock *)key, &key_schedule);
    
    DES_ecb_encrypt((const_DES_cblock *)ciphertext, (DES_cblock *)plaintext, &key_schedule, DES_DECRYPT);
    
    return 1;
}

int main() {
    // Example key (56 bits, ignoring parity bits)
    unsigned char key[8] = { 0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1 };
    
    // Example plaintext
    unsigned char plaintext[] = "Hello, DES!";
    int plaintext_len = strlen((const char *)plaintext);
    
    // Allocate memory for ciphertext (should be at least plaintext_len + 8)
    unsigned char ciphertext[plaintext_len + 8];
    
    // Encrypt the plaintext
    des_encrypt(plaintext, plaintext_len, ciphertext, key);
    
    // Print ciphertext
    printf("Ciphertext: ");
    print_hex(ciphertext, plaintext_len);
    
    // Allocate memory for decrypted plaintext
    unsigned char decrypted[plaintext_len + 1];
    
    // Decrypt the ciphertext
    des_decrypt(ciphertext, plaintext_len, decrypted, key);
    
    // Add a null terminator to decrypted data to print it safely
    decrypted[plaintext_len] = '\0';
    
    // Print decrypted plaintext
    printf("Decrypted plaintext: %s\n", decrypted);
    
    return 0;
}
