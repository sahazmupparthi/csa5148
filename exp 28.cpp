#include <stdio.h>
#include <stdlib.h>
#include <openssl/dh.h>
#include <openssl/engine.h>

void handleErrors(void)
{
    fprintf(stderr, "Error occurred.\n");
    exit(EXIT_FAILURE);
}

int main()
{
    DH *dh = NULL;
    int codes;
    unsigned char *secret;
    int secret_length;

    // Create parameters
    dh = DH_new();
    if (!dh) handleErrors();

    DH_generate_parameters_ex(dh, 2048, DH_GENERATOR_2, NULL);
    if (!DH_generate_key(dh)) handleErrors();

    // Alice's public key A
    BIGNUM *A = BN_dup(DH_get0_pub_key(dh));

    // Bob receives A and computes shared secret
    DH_compute_key(secret, A, dh);

    // Clean up
    DH_free(dh);
    BN_free(A);

    return 0;
}
