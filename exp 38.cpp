#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TEXT_LENGTH 100
#define ALPHABET_SIZE 26

// Function to convert a character to its corresponding index in the alphabet
int char_to_index(char ch) {
    if (isupper(ch)) {
        return ch - 'A';
    } else if (islower(ch)) {
        return ch - 'a';
    } else {
        return -1; // Not a valid alphabet character
    }
}

// Function to convert an index in the alphabet to a character
char index_to_char(int idx) {
    return 'A' + idx;
}

// Function to compute the modular multiplicative inverse of a number 'a' modulo m
// Returns -1 if the inverse does not exist
int mod_inverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // No modular inverse exists
}

// Function to calculate the determinant of a 2x2 matrix
int determinant_2x2(int mat[2][2]) {
    return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
}

// Function to calculate the modular determinant of a 2x2 matrix
int mod_determinant_2x2(int mat[2][2], int mod) {
    int det = determinant_2x2(mat);
    return (det % mod + mod) % mod; // Ensure positive modulo result
}

// Function to calculate the modular inverse of a 2x2 matrix
// Returns 1 if the inverse exists, 0 otherwise
int mod_inverse_2x2(int mat[2][2], int mod, int inverse[2][2]) {
    int det = mod_determinant_2x2(mat, mod);
    int det_inv = mod_inverse(det, mod);
    
    if (det_inv == -1) {
        return 0; // No modular inverse exists
    }
    
    // Calculate adjugate matrix
    inverse[0][0] = mat[1][1];
    inverse[0][1] = -mat[0][1];
    inverse[1][0] = -mat[1][0];
    inverse[1][1] = mat[0][0];
    
    // Multiply adjugate matrix by det_inv modulo mod
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            inverse[i][j] = (inverse[i][j] * det_inv) % mod;
            if (inverse[i][j] < 0) {
                inverse[i][j] += mod; // Ensure positive modulo result
            }
        }
    }
    
    return 1;
}

// Function to decrypt ciphertext using the Hill cipher with a known key matrix
void decrypt_hill_cipher(const char *ciphertext, int key[2][2], int mod) {
    int len = strlen(ciphertext);
    int decrypted_text[len];
    
    // Calculate inverse of the key matrix modulo mod
    int key_inverse[2][2];
    if (!mod_inverse_2x2(key, mod, key_inverse)) {
        printf("Error: Modular inverse of the key matrix does not exist.\n");
        return;
    }
    
    // Decrypt each pair of ciphertext characters
    for (int i = 0; i < len; i += 2) {
        int c1 = char_to_index(ciphertext[i]);
        int c2 = char_to_index(ciphertext[i + 1]);
        
        // Calculate plaintext indices using the key inverse
        int p1 = (key_inverse[0][0] * c1 + key_inverse[0][1] * c2) % mod;
        int p2 = (key_inverse[1][0] * c1 + key_inverse[1][1] * c2) % mod;
        
        // Ensure positive modulo results
        if (p1 < 0) p1 += mod;
        if (p2 < 0) p2 += mod;
        
        decrypted_text[i] = index_to_char(p1);
        decrypted_text[i + 1] = index_to_char(p2);
    }
    decrypted_text[len] = '\0';
    
    printf("Ciphertext: %s\n", ciphertext);
    printf("Key Matrix (mod %d):\n", mod);
    printf("%d %d\n%d %d\n", key[0][0], key[0][1], key[1][0], key[1][1]);
    printf("Decrypted plaintext: %s\n", decrypted_text);
}

int main() {
    const char *ciphertext = "DX";
    int key[2][2] = {{3, 2}, {5, 7}}; // Example key matrix
    int mod = ALPHABET_SIZE; // Modulo for Hill cipher is typically the size of the alphabet
    
    // Decrypt ciphertext using known key matrix
    decrypt_hill_cipher(ciphertext, key, mod);
    
    return 0;
}
