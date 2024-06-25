#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Function to calculate the greatest common divisor (GCD)
uint64_t gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to calculate modular exponentiation (base^exp % mod)
uint64_t modExp(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Function to find the private key d given e and phi(n)
uint64_t findPrivateKey(uint64_t e, uint64_t phi_n) {
    uint64_t d = 0;
    for (d = 2; d < phi_n; d++) {
        if ((e * d) % phi_n == 1) {
            break;
        }
    }
    return d;
}

// Function to factorize n using known plaintext attack
bool factorizeN(uint64_t n, uint64_t *p, uint64_t *q) {
    // Attempt to find factors of n
    for (uint64_t i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            *p = i;
            *q = n / i;
            return true;
        }
    }
    return false;
}

int main() {
    uint64_t n = 10403; // Example modulus n = p * q
    uint64_t e = 17;    // Example public exponent e

    // Assume we know one plaintext block has a common factor with n
    // Let's simulate this by trying to factorize n
    uint64_t p, q;
    bool success = factorizeN(n, &p, &q);

    if (!success) {
        printf("Unable to factorize n. No common factors found.\n");
        return 1;
    }

    printf("Factors of n found: p = %llu, q = %llu\n", p, q);

    // Compute phi(n)
    uint64_t phi_n = (p - 1) * (q - 1);

    // Find the private key d
    uint64_t d = findPrivateKey(e, phi_n);

    printf("Private key (d) = %llu\n", d);

    return 0;
}
