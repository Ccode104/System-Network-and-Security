#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>

// Function to generate a random prime number of given bit size
void generate_prime(mpz_t prime, unsigned int bits) {
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));
    
    // Generate a random prime number with specified bit length
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

// Function to find a suitable public exponent e
void find_coprime(mpz_t e, mpz_t phi) {
    mpz_set_ui(e, 65537); // Used a commonly used public exponent
}

// Function to perform RSA encryption: C = P^e mod n
void rsa_encrypt(mpz_t C, mpz_t P, mpz_t e, mpz_t n) {
    mpz_powm(C, P, e, n); // Compute C = P^e mod n
}

// Function to perform RSA decryption: P = C^d mod n
void rsa_decrypt(mpz_t P, mpz_t C, mpz_t d, mpz_t n) {
    mpz_powm(P, C, d, n); // Compute P = C^d mod n
}

int main() {
    mpz_t p, q, n, phi, e, d, P, C, decrypted_P;
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(phi);
    mpz_init(e);
    mpz_init(d);
    mpz_init(P);
    mpz_init(C);
    mpz_init(decrypted_P);
    
    generate_distinct_primes(p, q, 512);

    
    // Compute n = p * q
    mpz_mul(n, p, q);
    
    // Compute phi(n) = (p - 1) * (q - 1)
    mpz_t p_minus_1, q_minus_1;
    mpz_init(p_minus_1);
    mpz_init(q_minus_1);
    
    mpz_sub_ui(p_minus_1, p, 1);
    mpz_sub_ui(q_minus_1, q, 1);
    mpz_mul(phi, p_minus_1, q_minus_1);
    
    // Find public exponent e
    find_coprime(e, phi);
    
    // Compute private exponent d as modular inverse of e mod phi
    if (mpz_invert(d, e, phi) == 0) {
        printf("Error: Modular inverse does not exist!\n");
        return 1;
    }
    
    gmp_printf("Generated prime p: %Zd\n", p);
    gmp_printf("Generated prime q: %Zd\n", q);
    gmp_printf("Computed n: %Zd\n", n);
    gmp_printf("Computed phi(n): %Zd\n", phi);
    gmp_printf("Selected public exponent e: %Zd\n", e);
    gmp_printf("Computed private exponent d: %Zd\n", d);
    
    // Example plaintext message
    mpz_set_ui(P, 42); // Setting P to a small example value
    rsa_encrypt(C, P, e, n);
    
    gmp_printf("Plaintext P: %Zd\n", P);
    gmp_printf("Encrypted Ciphertext C: %Zd\n", C);
    
    // Perform decryption
    rsa_decrypt(decrypted_P, C, d, n);
    gmp_printf("Decrypted Plaintext P: %Zd\n", decrypted_P);
    
    if (mpz_cmp(P, decrypted_P) == 0) {
        printf("Decryption successful! P and decrypted_P match.\n");
    } else {
        printf("Decryption failed! P and decrypted_P do not match.\n");
    }
    
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(phi);
    mpz_clear(e);
    mpz_clear(d);
    mpz_clear(P);
    mpz_clear(C);
    mpz_clear(decrypted_P);
    mpz_clear(p_minus_1);
    mpz_clear(q_minus_1);
    
    return 0;
}