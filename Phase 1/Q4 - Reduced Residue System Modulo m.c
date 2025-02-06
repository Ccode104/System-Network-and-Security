#include <stdio.h>
#include<stdlib.h>
#include <gmp.h>

int main(int argc, char *argv[]) {
    // Initialize the modulus number (m)
    mpz_t modulus;
    mpz_init_set_ui(modulus, atoi(argv[1]));

    // Variable to iterate through numbers less than m
    mpz_t candidate;
    mpz_init(candidate);

    // Variable to store GCD calculation
    mpz_t gcd_value;
    mpz_init(gcd_value);

    // Counter for Euler's Totient function (φ(m))
    int totient_count = 1; 

    // The number 1 is always included in the Reduced Residue System (RRS)
    printf("1 ");

    // Loop through numbers from 2 to (m - 1)
    for (mpz_set_ui(candidate, 2); mpz_cmp(candidate, modulus) < 0; mpz_add_ui(candidate, candidate, 1)) {
        // Compute GCD of candidate and modulus
        mpz_gcd(gcd_value, modulus, candidate);

        // If GCD is 1, the number is part of RRSM_m
        if (mpz_cmp_ui(gcd_value, 1) == 0) {
            printf("%ld ", mpz_get_ui(candidate));
            totient_count++; // Increment φ(m)
        }
    }

    // Print Euler's Totient function value φ(m)
    printf("\nEuler Totient = %d\n", totient_count);

    // Free allocated memory
    mpz_clear(modulus);
    mpz_clear(candidate);
    mpz_clear(gcd_value);

    return 0;
}
