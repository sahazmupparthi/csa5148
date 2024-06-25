#include <stdio.h>
#include <stdint.h>

// Initial Permutation (IP)
const int IP[8] = {1, 5, 2, 0, 3, 7, 4, 6};

// Inverse Initial Permutation (IP-1)
const int IP_1[8] = {3, 0, 2, 4, 6, 1, 7, 5};

// Expansion Permutation (EP)
const int EP[8] = {3, 0, 1, 2, 1, 2, 3, 0};

// P10 and P8 for key generation
const int P10[10] = {2, 4, 1, 6, 3, 9, 0, 8, 7, 5};
const int P8[8] = {5, 2, 6, 3, 7, 4, 9, 8};

// P4 permutation
const int P4[4] = {1, 3, 2, 0};

// S-Boxes
const int S0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};

const int S1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

// Function prototypes
void sdesEncrypt(uint8_t *data, uint8_t *key);
void sdesDecrypt(uint8_t *data, uint8_t *key);
void generateSubkeys(uint8_t *key, uint8_t *k1, uint8_t *k2);
uint8_t f(uint8_t right, uint8_t subkey);
void permute(uint8_t *data, const int *table, int n);

void ctrEncryptDecrypt(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *key, uint8_t counter, int numBlocks);

void printBinary(uint8_t byte, int n);

int main() {
    uint8_t plaintext[] = {0x01, 0x02, 0x04};
    uint8_t key = 0x1FD; // 10-bit key: 01111 11101
    uint8_t counter = 0x00; // Starting counter
    int numBlocks = sizeof(plaintext);

    uint8_t ciphertext[numBlocks];
    uint8_t decrypted[numBlocks];

    ctrEncryptDecrypt(plaintext, ciphertext, &key, counter, numBlocks);
    printf("CTR Encrypted blocks:\n");
    for (int i = 0; i < numBlocks; i++) {
        printBinary(ciphertext[i], 8);
        printf("\n");
    }

    ctrEncryptDecrypt(ciphertext, decrypted, &key, counter, numBlocks);
    printf("\nCTR Decrypted blocks:\n");
    for (int i = 0; i < numBlocks; i++) {
        printBinary(decrypted[i], 8);
        printf("\n");
    }

    return 0;
}

void ctrEncryptDecrypt(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *key, uint8_t counter, int numBlocks) {
    for (int i = 0; i < numBlocks; i++) {
        uint8_t counterBlock = counter + i;
        sdesEncrypt(&counterBlock, key);
        ciphertext[i] = plaintext[i] ^ counterBlock;
    }
}

void sdesEncrypt(uint8_t *data, uint8_t *key) {
    uint8_t k1, k2;
    generateSubkeys(key, &k1, &k2);

    permute(data, IP, 8);
    uint8_t left = (*data & 0xF0) >> 4;
    uint8_t right = *data & 0x0F;

    uint8_t temp = f(right, k1);
    left ^= temp;

    uint8_t tmp = left;
    left = right;
    right = tmp;

    temp = f(right, k2);
    left ^= temp;

    *data = (left << 4) | right;
    permute(data, IP_1, 8);
}

void sdesDecrypt(uint8_t *data, uint8_t *key) {
    uint8_t k1, k2;
    generateSubkeys(key, &k1, &k2);

    permute(data, IP, 8);
    uint8_t left = (*data & 0xF0) >> 4;
    uint8_t right = *data & 0x0F;

    uint8_t temp = f(right, k2);
    left ^= temp;

    uint8_t tmp = left;
    left = right;
    right = tmp;

    temp = f(right, k1);
    left ^= temp;

    *data = (left << 4) | right;
    permute(data, IP_1, 8);
}

void generateSubkeys(uint8_t *key, uint8_t *k1, uint8_t *k2) {
    uint16_t key10 = *key & 0x3FF;
    uint16_t permKey = 0;

    for (int i = 0; i < 10; i++) {
        permKey |= ((key10 >> (9 - P10[i])) & 1) << (9 - i);
    }

    uint16_t left = (permKey & 0x3E0) >> 5;
    uint16_t right = permKey & 0x1F;

    left = ((left << 1) & 0x1F) | (left >> 4);
    right = ((right << 1) & 0x1F) | (right >> 4);

    uint16_t combined = (left << 5) | right;

    *k1 = 0;
    for (int i = 0; i < 8; i++) {
        *k1 |= ((combined >> (9 - P8[i])) & 1) << (7 - i);
    }

    left = ((left << 2) & 0x1F) | (left >> 3);
    right = ((right << 2) & 0x1F) | (right >> 3);

    combined = (left << 5) | right;

    *k2 = 0;
    for (int i = 0; i < 8; i++) {
        *k2 |= ((combined >> (9 - P8[i])) & 1) << (7 - i);
    }
}

uint8_t f(uint8_t right, uint8_t subkey) {
    uint8_t expandedRight = 0;

    for (int i = 0; i < 8; i++) {
        expandedRight |= ((right >> (3 - (EP[i] % 4))) & 1) << (7 - i);
    }

    expandedRight ^= subkey;

    uint8_t leftSBox = (expandedRight & 0xF0) >> 4;
    uint8_t rightSBox = expandedRight & 0x0F;

    uint8_t row = ((leftSBox & 0x08) >> 2) | (leftSBox & 0x01);
    uint8_t col = (leftSBox & 0x06) >> 1;
    uint8_t output = S0[row][col] << 2;

    row = ((rightSBox & 0x08) >> 2) | (rightSBox & 0x01);
    col = (rightSBox & 0x06) >> 1;
    output |= S1[row][col];

    uint8_t permOutput = 0;
    for (int i = 0; i < 4; i++) {
        permOutput |= ((output >> (3 - P4[i])) & 1) << (3 - i);
    }
    return permOutput;
}

void permute(uint8_t *data, const int *table, int n) {
    uint8_t temp = 0;
    for (int i = 0; i < n; i++) {
        temp |= ((*data >> (n - table[i] - 1)) & 1) << (n - i - 1);
    }
    *data = temp;
}

void printBinary(uint8_t byte, int n) {
    for (int i = n - 1; i >= 0; i--) {
        printf("%c", (byte & (1 << i)) ? '1' : '0');
    }
}
``
