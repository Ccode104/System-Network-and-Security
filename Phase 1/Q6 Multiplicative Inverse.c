#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

int main(int argc, char *argv[]) {
    // Initialize variables to store gcd, input number (a), and modulus (m)
    mpz_t gcd, a, m, inverse, temp;
    mpz_init(gcd);
    mpz_init_set_ui(a, atoi(argv[1])); // a (number)
    mpz_init_set_ui(m, atoi(argv[2])); // m (modulus)
    mpz_init(inverse);
    mpz_init(temp);

    // Compute gcd(a, m) and find modular inverse using Extended Euclidean Algorithm
    mpz_gcdext(gcd, inverse, temp, a, m);

    // Check if the multiplicative inverse exists (gcd(a, m) must be 1)
    if (mpz_cmp_ui(gcd, 1) == 0) {
        // Ensure the inverse is positive (modular inverse should be within [0, m-1])
        if (mpz_sgn(inverse) < 0) {
            mpz_add(inverse, inverse, m);
        }

        // Print result
        printf("Multiplicative inverse exists.\n");
        printf("The multiplicative inverse of %ld (mod %ld) is %ld (mod %ld)\n",
               mpz_get_ui(a), mpz_get_ui(m), mpz_get_ui(inverse), mpz_get_ui(m));
    } else {
        printf("Multiplicative inverse does not exist! (gcd(%ld, %ld) = %ld)\n",
               mpz_get_ui(a), mpz_get_ui(m), mpz_get_ui(gcd));
    }

    // Free allocated memory
    mpz_clear(gcd);
    mpz_clear(a);
    mpz_clear(m);
    mpz_clear(inverse);
    mpz_clear(temp);

    return 0;
}
