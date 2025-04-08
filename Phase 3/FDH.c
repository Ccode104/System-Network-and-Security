#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>
# include<string.h>
#include <openssl/sha.h>

// Function to generate a large random prime number
void generate_prime(mpz_t prime, unsigned int bits) {
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));
    
    mpz_urandomb(prime, state, bits);
    mpz_nextprime(prime, prime);
    
    gmp_randclear(state);
}
void generate_distinct_primes(mpz_t p, mpz_t q, unsigned int bits) {
    generate_prime(p, bits);
    do {
        generate_prime(q, bits);
    } while (mpz_cmp(p, q) == 0); // Ensure p != q
}

// Function to compute modular inverse
void compute_mod_inverse(mpz_t result, mpz_t e, mpz_t phi) {
    mpz_invert(result, e, phi);
}

// RSA Key Generation
void rsa_keygen(mpz_t n, mpz_t e, mpz_t d, unsigned int bits) {
    mpz_t p, q, phi;
    mpz_init(p);
    mpz_init(q);
    mpz_init(phi);
    
    generate_distinct_primes(p,q, bits / 2);
    
    mpz_mul(n, p, q);
    
    mpz_t p_1, q_1;
    mpz_init(p_1);
    mpz_init(q_1);
    
    mpz_sub_ui(p_1, p, 1);
    mpz_sub_ui(q_1, q, 1);
    mpz_mul(phi, p_1, q_1);
    
    mpz_set_ui(e, 65537);
    compute_mod_inverse(d, e, phi);
    
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(phi);
    mpz_clear(p_1);
    mpz_clear(q_1);
}

// SHA-256 Hash Function
void sha256_hash(mpz_t hash_val, const char* message) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)message, strlen(message), hash);
    
    mpz_import(hash_val, SHA256_DIGEST_LENGTH, 1, 1, 1, 0, hash);
}

// FDH Signature Generation
void fdh_sign(mpz_t signature, const char* message, mpz_t d, mpz_t n) {
    mpz_t hash_val;
    mpz_init(hash_val);
    sha256_hash(hash_val, message);
    
    mpz_powm(signature, hash_val, d, n);
    mpz_clear(hash_val);
}

// FDH Signature Verification
int fdh_verify(const char* message, mpz_t signature, mpz_t e, mpz_t n) {
    mpz_t hash_val, verified_hash;
    mpz_init(hash_val);
    mpz_init(verified_hash);
    
    sha256_hash(hash_val, message);
    
    mpz_powm(verified_hash, signature, e, n);
    
    int valid = mpz_cmp(hash_val, verified_hash) == 0;
    
    mpz_clear(hash_val);
    mpz_clear(verified_hash);
    
    return valid;
}

int main() {
    mpz_t n, e, d, signature;
    mpz_init(n);
    mpz_init(e);
    mpz_init(d);
    mpz_init(signature);
    
    rsa_keygen(n, e, d, 512);
    
    const char* message = "Hello, FDH!";
    fdh_sign(signature, message, d, n);
    
    gmp_printf("Message: %s\n", message);
    gmp_printf("Signature: %Zd\n", signature);
    
    if (fdh_verify(message, signature, e, n)) {
        printf("Signature is valid!\n");
    } else {
        printf("Signature verification failed!\n");
    }
    
    mpz_clear(n);
    mpz_clear(e);
    mpz_clear(d);
    mpz_clear(signature);
    
    return 0;
}
