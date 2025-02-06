#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

// NOTE : Here we use a^p-1 mod p = 1 mod p
// By replacing modulus_minus_one by modulus in the code and uncomment the line where we compute 'part 1' 
// you can see : a^p mod p = a mod p also
int main(int argc, char *argv[]) {
    // Initialize base (a), exponent (x), and modulus (n)
    mpz_t base, exponent, modulus;
    mpz_init_set_ui(base, atoi(argv[1]));
    mpz_init_set_ui(exponent, atoi(argv[2]));
    mpz_init_set_ui(modulus, atoi(argv[3]));

    // Print initial equation
    printf("Initial: %ld^%ld (mod %ld)\n", mpz_get_ui(base), mpz_get_ui(exponent), mpz_get_ui(modulus));

    // Check if modulus is prime (required for Fermat's theorem)
    printf("Checking if modulus is prime...\n");
    if (mpz_probab_prime_p(modulus, 50) == 2) {
        printf("Modulus is prime! Proceeding with Fermat's theorem.\n");
    } else {
        printf("Modulus is not prime. Fermat's theorem cannot be applied.\n");
        return 0;
    }

    // Initialize result variable
    mpz_t result;
    mpz_init(result);

    // If exponent < modulus, directly compute a^x mod n
    if (mpz_cmp(exponent, modulus) < 0) {
        mpz_powm(result, base, exponent, modulus);
        printf("Result: %ld\n", mpz_get_ui(result));
        return 0;
    }

    // Initialize variables for Fermat's theorem application
    mpz_t quotient, remainder;
    mpz_t part1, part2;
    mpz_init(quotient);
    mpz_init(remainder);
    mpz_init_set_ui(part1,1);
    mpz_init(part2);

    mpz_t modulus_minus_one;
    mpz_init(modulus_minus_one);

    mpz_sub_ui(modulus_minus_one,modulus,1);

    // Compute exponent as quotient * (modulus-1) + remainder
    mpz_fdiv_qr(quotient, remainder, exponent, modulus_minus_one);

    // Print breakdown of exponentiation
    printf("We have: (%ld^(%ld * %ld) + %ld) (mod %ld)\n", 
           mpz_get_ui(base), mpz_get_ui(quotient), mpz_get_ui(modulus_minus_one), mpz_get_ui(remainder), mpz_get_ui(modulus));

    printf("i.e. ((%ld^%ld)^%ld) * (%ld^%ld) (mod %ld)\n", 
           mpz_get_ui(base), mpz_get_ui(quotient), mpz_get_ui(modulus_minus_one), mpz_get_ui(base), mpz_get_ui(remainder), mpz_get_ui(modulus));

    printf("i.e. (((%ld^%ld)^%ld) (mod %ld)) * ((%ld^%ld) (mod %ld)) (mod %ld)\n", 
           mpz_get_ui(base), mpz_get_ui(quotient), mpz_get_ui(modulus_minus_one), mpz_get_ui(modulus), 
           mpz_get_ui(base), mpz_get_ui(remainder), mpz_get_ui(modulus), mpz_get_ui(modulus));

    printf("Using Fermat’s Theorem: a^p mod p = a\n");

    // Compute (base^quotient)^modulus mod modulus
    // mpz_powm(part1, base, quotient, modulus);
    printf("i.e. %ld * (%ld^%ld) mod %ld\n", mpz_get_ui(part1), mpz_get_ui(base), mpz_get_ui(remainder), mpz_get_ui(modulus));

    // Compute base^remainder mod modulus
    mpz_powm(part2, base, remainder, modulus);

    // Compute final result
    mpz_mul(result, part1, part2);
    printf("Result: %ld\n", mpz_get_ui(result));

    // Free allocated memory
    mpz_clear(base);
    mpz_clear(exponent);
    mpz_clear(modulus);
    mpz_clear(result);
    mpz_clear(quotient);
    mpz_clear(remainder);
    mpz_clear(part1);
    mpz_clear(part2);

    return 0;
}
