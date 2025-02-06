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
        mp_bitcnt_t n = mpz_remove(temp, temp, i);  // Factor out powers of i
        
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

// Function to compute the order of a modulo m
void compute_order(mpz_t order, mpz_t a, mpz_t m) {
    mpz_t temp, i, phi;
    mpz_init(temp);
    mpz_init_set_ui(i, 1);
    mpz_init(phi);

    // Compute Euler's totient function φ(m)
    compute_totient(phi, m);

    // Compute order by checking a^i ≡ 1 (mod m)
    mpz_set(temp, a);
    while (mpz_cmp(i, phi) <= 0) {
        if (mpz_cmp_ui(temp, 1) == 0) {
            mpz_set(order, i);
            break;
        }
        mpz_mul(temp, temp, a);
        mpz_mod(temp, temp, m);
        mpz_add_ui(i, i, 1);
    }

    mpz_clears(temp, i, phi, NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./a.out a m\n");
        return 1;
    }

    mpz_t a, m, gcd, order;
    mpz_inits(a, m, gcd, order, NULL);

    mpz_set_ui(a, atoi(argv[1]));
    mpz_set_ui(m, atoi(argv[2]));

    // Compute gcd(a, m)
    mpz_gcd(gcd, a, m);
    if (mpz_cmp_ui(gcd, 1) != 0) {
        printf("No order exists because gcd(%ld, %ld) ≠ 1.\n", mpz_get_ui(a), mpz_get_ui(m));
    } else {
        compute_order(order, a, m);
        printf("The order of %ld modulo %ld is %ld.\n", mpz_get_ui(a), mpz_get_ui(m), mpz_get_ui(order));
    }

    mpz_clears(a, m, gcd, order, NULL);
    return 0;
}
