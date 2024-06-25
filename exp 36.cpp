#include <stdio.h>
#include <ctype.h>

// Function to compute the greatest common divisor (GCD)
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to compute the modular inverse of a modulo m
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

// Function to encrypt a character using affine Caesar cipher
char encrypt_char(int a, int b, char plaintext) {
    if (isalpha(plaintext)) {
        char base = islower(plaintext) ? 'a' : 'A';
        int p = plaintext - base;
        int c = (a * p + b) % 26;
        return base + c;
    } else {
        return plaintext; // Return unchanged if not alphabetic
    }
}

// Function to decrypt a character using affine Caesar cipher
char decrypt_char(int a, int b, char ciphertext) {
    if (isalpha(ciphertext)) {
        char base = islower(ciphertext) ? 'a' : 'A';
        int c = ciphertext - base;
        int a_inv = mod_inverse(a, 26);
        if (a_inv == -1) {
            printf("Error: Modular inverse of 'a' does not exist.\n");
            return ciphertext;
        }
        int p = (a_inv * (c - b + 26)) % 26;
        return base + p;
    } else {
        return ciphertext; // Return unchanged if not alphabetic
    }
}

// Function to encrypt plaintext using affine Caesar cipher
void encrypt_affine_caesar(int a, int b, const char *plaintext) {
    printf("Plaintext: %s\n", plaintext);
    printf("Key: [a = %d, b = %d]\n", a, b);
    printf("Ciphertext: ");
    
    for (int i = 0; plaintext[i] != '\0'; i++) {
        char encrypted_char = encrypt_char(a, b, plaintext[i]);
        printf("%c", encrypted_char);
    }
    printf("\n");
}

// Function to decrypt ciphertext using affine Caesar cipher
void decrypt_affine_caesar(int a, int b, const char *ciphertext) {
    printf("Ciphertext: %s\n", ciphertext);
    printf("Key: [a = %d, b = %d]\n", a, b);
    printf("Decrypted plaintext: ");
    
    int a_inv = mod_inverse(a, 26);
    if (a_inv == -1) {
        printf("Error: Modular inverse of 'a' does not exist. Cannot decrypt.\n");
        return;
    }
    
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        char decrypted_char = decrypt_char(a, b, ciphertext[i]);
        printf("%c", decrypted_char);
    }
    printf("\n");
}

int main() {
    int a = 3; // Example values of a and b
    int b = 7;
    const char *plaintext = "Hello, Affine Caesar Cipher!";
    
    // Encrypt plaintext
    encrypt_affine_caesar(a, b, plaintext);
    
    printf("\n");
    
    // Example ciphertext (to be decrypted)
    const char *ciphertext = "Khoor, Jllihv Fhduhw!";
    
    // Decrypt ciphertext
    decrypt_affine_caesar(a, b, ciphertext);
    
    return 0;
}
