#include <stdio.h>
#include <stdint.h>

// Extended Euclidean algorithm to find d such that e*d = 1 (mod phi)
uint64_t extendedEuclidean(uint64_t e, uint64_t phi) {
    int64_t d = 0, old_d = 1;
    int64_t r = phi, old_r = e;
    int64_t quotient, temp;

    while (r != 0) {
        quotient = old_r / r;

        temp = r;
        r = old_r - quotient * r;
        old_r = temp;

        temp = d;
        d = old_d - quotient * d;
        old_d = temp;
    }

    // Make sure d is positive
    if (old_d < 0) {
        old_d += phi;
    }

    return old_d;
}

int main() {
    uint64_t e = 31;
    uint64_t n = 3599;
    uint64_t p = 59, q = 61; // From factorization of n
    uint64_t phi_n = (p - 1) * (q - 1);
    
    uint64_t d = extendedEuclidean(e, phi_n);

    printf("Private key (d) = %llu\n", d);

    return 0;
}
