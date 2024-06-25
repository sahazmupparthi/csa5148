#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Define the block size for simplicity
#define BLOCK_SIZE 8

// Dummy encryption/decryption functions
void dummyEncrypt(uint64_t* block, uint64_t key) {
    *block ^= key;
}

void dummyDecrypt(uint64_t* block, uint64_t key) {
    *block ^= key;
}

// Padding function
void padMessage(uint8_t* message, int messageLen, int blockSize, int* paddedLen) {
    int padLen = blockSize - (messageLen % blockSize);
    *paddedLen = messageLen + padLen;
    message[messageLen] = 0x80; // 1 bit followed by 0s
    for (int i = 1; i < padLen; i++) {
        message[messageLen + i] = 0x00;
    }
}

// ECB Mode
void ecbEncrypt(uint8_t* plaintext, uint8_t* ciphertext, uint64_t key, int numBlocks) {
    for (int i = 0; i < numBlocks; i++) {
        uint64_t block;
        memcpy(&block, &plaintext[i * BLOCK_SIZE], BLOCK_SIZE);
        dummyEncrypt(&block, key);
        memcpy(&ciphertext[i * BLOCK_SIZE], &block, BLOCK_SIZE);
    }
}

void ecbDecrypt(uint8_t* ciphertext, uint8_t* plaintext, uint64_t key, int numBlocks) {
    for (int i = 0; i < numBlocks; i++) {
        uint64_t block;
        memcpy(&block, &ciphertext[i * BLOCK_SIZE], BLOCK_SIZE);
        dummyDecrypt(&block, key);
        memcpy(&plaintext[i * BLOCK_SIZE], &block, BLOCK_SIZE);
    }
}

// CBC Mode
void cbcEncrypt(uint8_t* plaintext, uint8_t* ciphertext, uint64_t key, uint64_t iv, int numBlocks) {
    uint64_t prevCiphertext = iv;
    for (int i = 0; i < numBlocks; i++) {
        uint64_t block;
        memcpy(&block, &plaintext[i * BLOCK_SIZE], BLOCK_SIZE);
        block ^= prevCiphertext;
        dummyEncrypt(&block, key);
        memcpy(&ciphertext[i * BLOCK_SIZE], &block, BLOCK_SIZE);
        prevCiphertext = block;
    }
}

void cbcDecrypt(uint8_t* ciphertext, uint8_t* plaintext, uint64_t key, uint64_t iv, int numBlocks) {
    uint64_t prevCiphertext = iv;
    for (int i = 0; i < numBlocks; i++) {
        uint64_t block;
        memcpy(&block, &ciphertext[i * BLOCK_SIZE], BLOCK_SIZE);
        uint64_t temp = block;
        dummyDecrypt(&block, key);
        block ^= prevCiphertext;
        memcpy(&plaintext[i * BLOCK_SIZE], &block, BLOCK_SIZE);
        prevCiphertext = temp;
    }
}

// CFB Mode
void cfbEncrypt(uint8_t* plaintext, uint8_t* ciphertext, uint64_t key, uint64_t iv, int numBlocks) {
    uint64_t prevCiphertext = iv;
    for (int i = 0; i < numBlocks; i++) {
        dummyEncrypt(&prevCiphertext, key);
        uint64_t block;
        memcpy(&block, &plaintext
