#include <stdio.h>
#include <stdint.h>

// Permuted Choice 1 Table
int PC1_A[28] = { ... }; // Fill in first subset of PC1 for first 24 bits
int PC1_B[28] = { ... }; // Fill in second subset of PC1 for second 24 bits

// Permuted Choice 2 Table
int PC2_A[24] = { ... }; // Fill in PC2 for first subset
int PC2_B[24] = { ... }; // Fill in PC2 for second subset

// Shifts for each round
int shifts[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

// Function prototypes
void permute(int* table, int size, uint64_t input, uint64_t* output);
void generateSubkeys(uint64_t key, uint64_t* subkeys);

int main() {
    uint64_t key = 0x133457799BBCDFF1; // Example key
    uint64_t subkeys[16];

    generateSubkeys(key, subkeys);

    for (int i = 0; i < 16; i++) {
        printf("Subkey %d: %012llx\n", i + 1, subkeys[i]);
    }

    return 0;
}

void permute(int* table, int size, uint64_t input, uint64_t* output) {
    *output = 0;
    for (int i = 0; i < size; i++) {
        *output |= ((input >> (64 - table[i])) & 1) << (size - 1 - i);
    }
}

void generateSubkeys(uint64_t key, uint64_t* subkeys) {
    uint32_t C, D;

    permute(PC1_A, 28, key, (uint64_t*)&C);
    permute(PC1_B, 28, key, (uint64_t*)&D);

    for (int i = 0; i < 16; i++) {
        C = ((C << shifts[i]) | (C >> (28 - shifts[i]))) & 0xFFFFFFF;
        D = ((D << shifts[i]) | (D >> (28 - shifts[i]))) & 0xFFFFFFF;

        uint64_t combined = ((uint64_t)C << 28) | D;
        uint64_t subkeyA, subkeyB;

        permute(PC2_A, 24, (uint64_t)C, &subkeyA);
        permute(PC2_B, 24, (uint64_t)D, &subkeyB);

        subkeys[i] = (subkeyA << 24) | subkeyB;
    }
}
