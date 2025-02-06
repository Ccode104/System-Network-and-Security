#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

// Function to check if all moduli are pairwise coprime
int are_coprime(mpz_t m[], int n) {
    mpz_t gcd;
    mpz_init(gcd);
    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            mpz_gcd(gcd, m[i], m[j]);
            if (mpz_cmp_ui(gcd, 1) != 0) {
                mpz_clear(gcd);
                return 0; // Not coprime
            }
        }
    }
    mpz_clear(gcd);
    return 1; // All are coprime
}

// Function implementing Chinese Remainder Theorem (CRT)
void solve_crt(mpz_t x, mpz_t a[], mpz_t m[], int n) {
    mpz_t M_prod;
    mpz_init_set_ui(M_prod, 1);

    // Compute product of all moduli
    for (int i = 0; i < n; i++) {
        mpz_mul(M_prod, M_prod, m[i]);
    }

    mpz_t M[n], inv, temp;
    mpz_init(inv);
    mpz_init(temp);
    mpz_set_ui(x, 0); // Initialize solution x to 0

    // Compute x using CRT formula
    for (int i = 0; i < n; i++) {
        mpz_init(M[i]);
        mpz_fdiv_q(M[i], M_prod, m[i]); // M[i] = M / m[i]
        
        // Compute modular inverse of M[i] mod m[i]
        if (mpz_invert(inv, M[i], m[i]) == 0) {
            printf("No modular inverse found. CRT solution does not exist.\n");
            return;
        }

        // temp = M[i] * inv * a[i]
        mpz_mul(temp, M[i], inv);
        mpz_mul(temp, temp, a[i]);

        // x = x + temp
        mpz_add(x, x, temp);
    }

    // Compute final result x = x % M_prod
    mpz_mod(x, x, M_prod);

    // Print solution
    printf("Solution: x ≡ %ld (mod %ld)\n", mpz_get_ui(x), mpz_get_ui(M_prod));

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        mpz_clear(M[i]);
    }
    mpz_clears(M_prod, inv, temp, NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 3 || (argc - 1) % 2 != 0) {
        printf("Usage: ./a.out a1 m1 a2 m2 ... an mn\n");
        return 1;
    }

    int n = (argc - 1) / 2;

    // Initialize arrays for a and m
    mpz_t a[n], m[n];
    for (int i = 0; i < n; i++) {
        mpz_init_set_ui(a[i], atoi(argv[2 * i + 1]));
        mpz_init_set_ui(m[i], atoi(argv[2 * i + 2]));
    }

    // Check if moduli are pairwise coprime
    if (!are_coprime(m, n)) {
        printf("Moduli are not pairwise coprime. CRT cannot be applied.\n");
        
        // Free allocated memory
        for (int i = 0; i < n; i++) {
            mpz_clears(a[i], m[i], NULL);
        }
        return 1;
    }

    // Compute solution using CRT
    mpz_t x;
    mpz_init(x);
    solve_crt(x, a, m, n);

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        mpz_clears(a[i], m[i], NULL);
    }
    mpz_clear(x);

    return 0;
}
