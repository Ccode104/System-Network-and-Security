#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>

// Function to generate a large random prime number
void generate_prime(mpz_t prime, unsigned int bits) {
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));
    
    // Generate a random prime number with specified bit length
    mpz_urandomb(prime, state, bits);
    mpz_nextprime(prime, prime);
    
    gmp_randclear(state);
}

// Function to generate a random number in the range [1, p-2]
void generate_random(mpz_t rand_num, mpz_t p) {
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));
    
    mpz_sub_ui(rand_num, p, 2); // rand_num = p - 2
    mpz_urandomm(rand_num, state, rand_num); // rand_num in range [0, p-3]
    mpz_add_ui(rand_num, rand_num, 1); // Shift to range [1, p-2]
    
    gmp_randclear(state);
}

// Function to compute e2 = e1^d mod p
void compute_public_key(mpz_t e2, mpz_t e1, mpz_t d, mpz_t p) {
    mpz_powm(e2, e1, d, p);
}

// Function to perform ElGamal Encryption
void elgamal_encrypt(mpz_t C1, mpz_t C2, mpz_t e1, mpz_t e2, mpz_t p, mpz_t P) {
    mpz_t r, e2_r;
    mpz_init(r);
    mpz_init(e2_r);
    
    // Select a random integer r in range [1, p-2]
    generate_random(r, p);
    
    // Compute C1 = e1^r mod p
    mpz_powm(C1, e1, r, p);
    
    // Compute e2^r mod p
    mpz_powm(e2_r, e2, r, p);
    
    // Compute C2 = (P * e2^r) mod p
    mpz_mul(C2, P, e2_r);
    mpz_mod(C2, C2, p);
    
    mpz_clear(r);
    mpz_clear(e2_r);
}

// Function to perform ElGamal Decryption
void elgamal_decrypt(mpz_t P, mpz_t C1, mpz_t C2, mpz_t d, mpz_t p) {
    mpz_t C1_d, C1_d_inv;
    mpz_init(C1_d);
    mpz_init(C1_d_inv);
    
    // Compute C1^d mod p
    mpz_powm(C1_d, C1, d, p);
    
    // Compute (C1^d)^-1 mod p
    mpz_invert(C1_d_inv, C1_d, p);
    
    // Compute P = (C2 * C1_d_inv) mod p
    mpz_mul(P, C2, C1_d_inv);
    mpz_mod(P, P, p);
    
    mpz_clear(C1_d);
    mpz_clear(C1_d_inv);
}

int main() {
    mpz_t p, d, e1, e2, P, C1, C2, decrypted_P;
    mpz_init(p);
    mpz_init(d);
    mpz_init(e1);
    mpz_init(e2);
    mpz_init(P);
    mpz_init(C1);
    mpz_init(C2);
    mpz_init(decrypted_P);
    
    // Step 1: Generate a large prime p
    generate_prime(p, 512);
    
    // Step 2: Select private key d in range [1, p-2]
    generate_random(d, p);
    
    // Step 3: Select primitive root e1 (for simplicity, setting e1 = 2)
    mpz_set_ui(e1, 2);
    
    // Step 4: Compute e2 = e1^d mod p
    compute_public_key(e2, e1, d, p);
    
    gmp_printf("Generated prime p: %Zd\n", p);
    gmp_printf("Private key d: %Zd\n", d);
    gmp_printf("Primitive root e1: %Zd\n", e1);
    gmp_printf("Computed public key e2: %Zd\n", e2);
    
    // Example plaintext message P
    mpz_set_ui(P, 42);
    
    // Perform encryption
    elgamal_encrypt(C1, C2, e1, e2, p, P);
    
    gmp_printf("Plaintext P: %Zd\n", P);
    gmp_printf("Ciphertext C1: %Zd\n", C1);
    gmp_printf("Ciphertext C2: %Zd\n", C2);
    
    // Perform decryption
    elgamal_decrypt(decrypted_P, C1, C2, d, p);
    gmp_printf("Decrypted Plaintext P: %Zd\n", decrypted_P);
    
    // Clear memory
    mpz_clear(p);
    mpz_clear(d);
    mpz_clear(e1);
    mpz_clear(e2);
    mpz_clear(P);
    mpz_clear(C1);
    mpz_clear(C2);
    mpz_clear(decrypted_P);
    
    return 0;
}
