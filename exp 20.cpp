#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Dummy encryption/decryption functions for illustration
void dummyEncrypt(uint64_t* block, uint64_t key) {
    *block ^= key;
}

void dummyDecrypt(uint64_t* block, uint64_t key) {
    *block ^= key;
}

// ECB Mode
void ecbEncrypt(uint64_t* plaintext, uint64_t* ciphertext, uint64_t key, int numBlocks) {
    for (int i = 0; i < numBlocks; i++) {
        ciphertext[i] = plaintext[i];
        dummyEncrypt(&ciphertext[i], key);
    }
}

void ecbDecrypt(uint64_t* ciphertext, uint64_t* plaintext, uint64_t key, int numBlocks) {
    for (int i = 0; i < numBlocks; i++) {
        plaintext[i] = ciphertext[i];
        dummyDecrypt(&plaintext[i], key);
    }
}

// CBC Mode
void cbcEncrypt(uint64_t* plaintext, uint64_t* ciphertext, uint64_t key, uint64_t iv, int numBlocks) {
    uint64_t prevCiphertext = iv;
    for (int i = 0; i < numBlocks; i++) {
        plaintext[i] ^= prevCiphertext;
        ciphertext[i] = plaintext[i];
        dummyEncrypt(&ciphertext[i], key);
        prevCiphertext = ciphertext[i];
    }
}

void cbcDecrypt(uint64_t* ciphertext, uint64_t* plaintext, uint64_t key, uint64_t iv, int numBlocks) {
    uint64_t prevCiphertext = iv;
    for (int i = 0; i < numBlocks; i++) {
        uint64_t temp = ciphertext[i];
        dummyDecrypt(&plaintext[i], key);
        plaintext[i] ^= prevCiphertext;
        prevCiphertext = temp;
    }
}

int main() {
    uint64_t key = 0x0123456789ABCDEF;
    uint64_t iv = 0x1234567890ABCDEF;
    uint64_t plaintext[] = {
        0x0123456789ABCDEF,
        0xFEDCBA9876543210,
        0x0F0F0F0F0F0F0F0F
    };
    int numBlocks = sizeof(plaintext) / sizeof(plaintext[0]);
    uint64_t ciphertextECB[numBlocks];
    uint64_t ciphertextCBC[numBlocks];
    uint64_t decryptedECB[numBlocks];
    uint64_t decryptedCBC[numBlocks];

    // Encrypt using ECB
    ecbEncrypt(plaintext, ciphertextECB, key, numBlocks);
    printf("ECB Encrypted blocks:\n");
    for (int i = 0; i < numBlocks; i++) {
        printf("%016llx\n", ciphertextECB[i]);
    }

    // Encrypt using CBC
    cbcEncrypt(plaintext, ciphertextCBC, key, iv, numBlocks);
    printf("\nCBC Encrypted blocks:\n");
    for (int i = 0; i < numBlocks; i++) {
        printf("%016llx\n", ciphertextCBC[i]);
    }

    // Introduce an error in the first ciphertext block
    ciphertextECB[0] ^= 0x01;
    ciphertextCBC[0] ^= 0x01;

    // Decrypt using ECB
    ecbDecrypt(ciphertextECB, decryptedECB, key, numBlocks);
    printf("\nECB Decrypted blocks (with error):\n");
    for (int i = 0; i < numBlocks; i++) {
        printf("%016llx\n", decryptedECB[i]);
    }

    // Decrypt using CBC
    cbcDecrypt(ciphertextCBC, decryptedCBC, key, iv, numBlocks);
    printf("\nCBC Decrypted blocks (with error):\n");
    for (int i = 0; i < numBlocks; i++) {
        printf("%016llx\n", decryptedCBC[i]);
    }

    return 0;
}
