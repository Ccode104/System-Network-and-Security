#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Initialize variables for number and prime factor
    mpz_t number, prime_factor;
    
    // Set the number from input argument and initialize the first prime factor as 2
    mpz_init_set_ui(number, atoi(argv[1]));
    mpz_init_set_ui(prime_factor, 2);

    // Perform prime factorization
    while (mpz_cmp(prime_factor, number) <= 0) {
        // Check if the current prime_factor divides number
        if (mpz_divisible_p(number, prime_factor)) {
            // Print the prime factor
            printf("%ld ", mpz_get_ui(prime_factor));
            // Divide number by the found prime factor
            mpz_div(number, number, prime_factor);
        } else {
            // Increment the factor to check the next number
            mpz_add_ui(prime_factor, prime_factor, 1);
        }
    }

    // Clear memory used by GMP variables
    mpz_clear(number);
    mpz_clear(prime_factor);

    return 0;
}
