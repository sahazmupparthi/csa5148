#include <stdio.h>

// Define the constant C for 128-bit block size
#define CONSTANT_C 0x87

// Function to perform left shift operation on a byte array
void left_shift(unsigned char *input, unsigned char *output, int len) {
    int i;
    unsigned char carry = 0;
    
    for (i = 0; i < len; ++i) {
        output[i] = (input[i] << 1) | carry;
        carry = (input[i] & 0x80) ? 1 : 0; // Check if MSB is set
    }
    
    if (carry) {
        output[len - 1] ^= CONSTANT_C; // XOR with constant C if necessary
    }
}

int main() {
    unsigned char L[16] = {0};  // Initialize L as a block of 128 bits (16 bytes)
    unsigned char K1[16] = {0}; // To store the first subkey K1
    unsigned char K2[16] = {0}; // To store the second subkey K2
    
    // Encrypt block of all zeros to get L
    // In this example, L is initialized as all zeros
    
    // Generate K1
    left_shift(L, K1, 16);
    
    // Generate K2
    left_shift(K1, K2, 16);
    
    // Print K1 and K2 (for demonstration purposes)
    printf("K1: ");
    for (int i = 0; i < 16; ++i) {
        printf("%02X ", K1[i]);
    }
    printf("\n");
    
    printf("K2: ");
    for (int i = 0; i < 16; ++i) {
        printf("%02X ", K2[i]);
    }
    printf("\n");
    
    return 0;
}
