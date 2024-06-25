#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/dsa.h>
#include <openssl/sha.h>

// Function to generate DSA keys
DSA* generate_dsa_keys() {
    DSA *dsa = DSA_new();
    if (!dsa) {
        perror("DSA_new");
        return NULL;
    }
    
    // Generate DSA parameters
    if (!DSA_generate_parameters_ex(dsa, 1024, NULL, 0, NULL, NULL, NULL)) {
        perror("DSA_generate_parameters_ex");
        DSA_free(dsa);
        return NULL;
    }
    
    // Generate DSA keys
    if (!DSA_generate_key(dsa)) {
        perror("DSA_generate_key");
        DSA_free(dsa);
        return NULL;
    }
    
    return dsa;
}

// Function to sign a message using DSA
int dsa_sign(DSA *dsa, const unsigned char *message, size_t message_len,
             unsigned char **sig, unsigned int *sig_len) {
    if (!dsa || !message || !sig || !sig_len) {
        fprintf(stderr, "Invalid input parameters.\n");
        return 0;
    }
    
    // Allocate memory for the signature
    *sig = (unsigned char *)malloc(DSA_size(dsa));
    if (!*sig) {
        perror("malloc");
        return 0;
    }
    
    // Calculate the hash of the message (using SHA-1 here for simplicity)
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(message, message_len, hash);
    
    // Sign the hash using DSA
    if (!DSA_sign(0, hash, SHA_DIGEST_LENGTH, *sig, sig_len, dsa)) {
        perror("DSA_sign");
        free(*sig);
        return 0;
    }
    
    return 1;
}

// Function to verify a DSA signature
int dsa_verify(DSA *dsa, const unsigned char *message, size_t message_len,
               const unsigned char *sig, unsigned int sig_len) {
    if (!dsa || !message || !sig) {
        fprintf(stderr, "Invalid input parameters.\n");
        return 0;
    }
    
    // Calculate the hash of the message (using SHA-1 here for simplicity)
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(message, message_len, hash);
    
    // Verify the signature
    int verified = DSA_verify(0, hash, SHA_DIGEST_LENGTH, sig, sig_len, dsa);
    if (verified == 1) {
        printf("Signature verified successfully.\n");
        return 1;
    } else if (verified == 0) {
        printf("Signature verification failed.\n");
        return 0;
    } else {
        perror("DSA_verify");
        return 0;
    }
}

int main() {
    DSA *dsa = generate_dsa_keys();
    if (!dsa) {
        fprintf(stderr, "Error generating DSA keys.\n");
        return 1;
    }
    
    // Message to be signed
    unsigned char message[] = "This is a message to be signed.";
    size_t message_len = strlen((const char *)message);
    
    // Variables for signature
    unsigned char *signature = NULL;
    unsigned int signature_len = 0;
    
    // Sign the message
    if (!dsa_sign(dsa, message, message_len, &signature, &signature_len)) {
        fprintf(stderr, "Error signing the message.\n");
        DSA_free(dsa);
        return 1;
    }
    
    // Verify the signature
    if (!dsa_verify(dsa, message, message_len, signature, signature_len)) {
        fprintf(stderr, "Error verifying the signature.\n");
    }
    
    // Clean up
    DSA_free(dsa);
    free(signature);
    
    return 0;
}
