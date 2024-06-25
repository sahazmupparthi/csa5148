#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Function to encrypt plaintext using one-time pad Vigenère cipher
void encrypt_vigenere_one_time_pad(const char *plaintext, const int *key, int key_len) {
    int plaintext_len = strlen(plaintext);
    char ciphertext[plaintext_len + 1];
    
    for (int i = 0; i < plaintext_len; ++i) {
        char ch = plaintext[i];
        if (isalpha(ch)) {
            // Calculate the shift based on the key
            int shift = key[i % key_len];
            
            // Adjust shift to be within 0-25
            if (islower(ch)) {
                ciphertext[i] = 'a' + (ch - 'a' + shift) % 26;
            } else if (isupper(ch)) {
                ciphertext[i] = 'A' + (ch - 'A' + shift) % 26;
            }
        } else {
            // Non-alphabetic characters remain unchanged
            ciphertext[i] = ch;
        }
    }
    ciphertext[plaintext_len] = '\0';
    
    printf("Plaintext: %s\n", plaintext);
    printf("Key: ");
    for (int i = 0; i < key_len; ++i) {
        printf("%d ", key[i]);
    }
    printf("\n");
    printf("Ciphertext: %s\n", ciphertext);
}

// Function to decrypt ciphertext encrypted with one-time pad Vigenère cipher
void decrypt_vigenere_one_time_pad(const char *ciphertext, const int *key, int key_len) {
    int ciphertext_len = strlen(ciphertext);
    char plaintext[ciphertext_len + 1];
    
    for (int i = 0; i < ciphertext_len; ++i) {
        char ch = ciphertext[i];
        if (isalpha(ch)) {
            // Calculate the reverse shift based on the key
            int shift = key[i % key_len];
            
            // Adjust shift to be within 0-25
            if (islower(ch)) {
                plaintext[i] = 'a' + (ch - 'a' - shift + 26) % 26;
            } else if (isupper(ch)) {
                plaintext[i] = 'A' + (ch - 'A' - shift + 26) % 26;
            }
        } else {
            // Non-alphabetic characters remain unchanged
            plaintext[i] = ch;
        }
    }
    plaintext[ciphertext_len] = '\0';
    
    printf("Ciphertext: %s\n", ciphertext);
    printf("Key: ");
    for (int i = 0; i < key_len; ++i) {
        printf("%d ", key[i]);
    }
    printf("\n");
    printf("Decrypted plaintext: %s\n", plaintext);
}

int main() {
    // Example plaintext and key
    const char *plaintext = "Hello, World!";
    int key[] = { 3, 19, 5 }; // Example key
    
    int key_len = sizeof(key) / sizeof(key[0]);
    
    // Encrypt using one-time pad Vigenère cipher
    encrypt_vigenere_one_time_pad(plaintext, key, key_len);
    
    printf("\n");
    
    // Example ciphertext (to be decrypted)
    const char *ciphertext = "Khoos, Hcuux!";
    
    // Decrypt using one-time pad Vigenère cipher
    decrypt_vigenere_one_time_pad(ciphertext, key, key_len);
    
    return 0;
}
