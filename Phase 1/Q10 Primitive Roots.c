#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

// Function to compute Euler's totient function (φ(m))
void compute_totient(mpz_t phi, mpz_t m) {
    mpz_t temp, i, temp2;
    mpz_init_set(temp, m);
    mpz_init_set_ui(phi, 1);
    mpz_init(temp2);
    mpz_init_set_ui(i, 2);
    
    while (mpz_cmp_ui(temp, 1) != 0) {
        mp_bitcnt_t n = mpz_remove(temp, temp, i);
        if (n == 0) {
            mpz_add_ui(i, i, 1);
            continue;
        }
        mpz_pow_ui(temp2, i, n - 1);
        mpz_submul(temp2, temp2, i);
        mpz_neg(temp2, temp2);
        mpz_mul(phi, phi, temp2);
    }

    mpz_clears(temp, i, temp2, NULL);
}

// Function to check if m is of valid form for primitive roots
int is_valid_m(mpz_t m) {
    if (mpz_cmp_ui(m, 2) == 0 || mpz_cmp_ui(m, 4) == 0) {
        return 1;
    }
    
    mpz_t temp, i;
    mpz_init_set(temp, m);
    mpz_init_set_ui(i, 3);
    
    while (mpz_cmp(i, m) <= 0) {
        mp_bitcnt_t n = mpz_remove(temp, temp, i);
        if (mpz_cmp_ui(temp, 2) == 0 || mpz_cmp_ui(temp, 1) == 0) {
            mpz_clears(temp, i, NULL);
            return 1;
        }
        mpz_add_ui(i, i, 2);
        mpz_set(temp, m);
    }

    mpz_clears(temp, i, NULL);
    return 0;
}

// Function to check if a number is a primitive root modulo m
int is_primitive_root(mpz_t g, mpz_t m, mpz_t phi) {
    mpz_t temp, gcd, pow_result, d;
    mpz_init(temp);
    mpz_init(gcd);
    mpz_init(pow_result);
    mpz_init_set_ui(d, 2);

    // Check gcd(g, m) == 1
    mpz_gcd(gcd, g, m);
    if (mpz_cmp_ui(gcd, 1) != 0) {
        mpz_clears(temp, gcd, pow_result, d, NULL);
        return 0;
    }

    // Check g^d ≠ 1 (mod m) for all divisors d of φ(m)
    while (mpz_cmp(d, phi) < 0) {
        if (mpz_divisible_p(phi, d)) {
            mpz_powm(pow_result, g, d, m);
            if (mpz_cmp_ui(pow_result, 1) == 0) {
                mpz_clears(temp, gcd, pow_result, d, NULL);
                return 0;
            }
        }
        mpz_add_ui(d, d, 1);
    }

    mpz_clears(temp, gcd, pow_result, d, NULL);
    return 1;
}

// Main function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./a.out m\n");
        return 1;
    }

    mpz_t m, phi, g, count;
    mpz_inits(m, phi, g, count, NULL);

    mpz_set_ui(m, atoi(argv[1]));

    // Check if m is valid for primitive roots
    if (!is_valid_m(m)) {
        printf("No primitive roots exist for m = %ld.\n", mpz_get_ui(m));
        mpz_clears(m, phi, g, count, NULL);
        return 0;
    }

    // Compute φ(m)
    compute_totient(phi, m);

    // Find all primitive roots
    mpz_set_ui(g, 1);
    mpz_set_ui(count, 0);

    printf("Primitive roots modulo %ld:\n", mpz_get_ui(m));
    while (mpz_cmp(g, m) < 0) {
        if (is_primitive_root(g, m, phi)) {
            printf("%ld ", mpz_get_ui(g));
            mpz_add_ui(count, count, 1);
        }
        mpz_add_ui(g, g, 1);
    }

    printf("\nTotal number of primitive roots: %ld\n", mpz_get_ui(count));

    mpz_clears(m, phi, g, count, NULL);
    return 0;
}
