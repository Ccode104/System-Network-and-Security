#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

int main(int argc, char *argv[]) {
    // Initialize variables for coefficients (a, b), modulus (m), and gcd (g)
    mpz_t a, b, m, gcd, inverse_a, solution;
    mpz_inits(a, b, m, gcd, inverse_a, solution, NULL);

    // Read input values
    mpz_set_ui(a, atoi(argv[1])); // Coefficient of x
    mpz_set_ui(b, atoi(argv[2])); // Right-hand side value
    mpz_set_ui(m, atoi(argv[3])); // Modulus

    // Compute gcd(a, m)
    mpz_gcd(gcd, a, m);

    // Check if b is divisible by gcd(a, m), otherwise no solution exists
    if (mpz_divisible_p(b, gcd) == 0) {
        printf("No solution exists! (gcd(%ld, %ld) = %ld does not divide %ld)\n",
               mpz_get_ui(a), mpz_get_ui(m), mpz_get_ui(gcd), mpz_get_ui(b));

        // Free allocated memory and exit
        mpz_clears(a, b, m, gcd, inverse_a, solution, NULL);
        return 0;
    }

    // Reduce equation by dividing all terms by gcd
    mpz_fdiv_q(a, a, gcd);
    mpz_fdiv_q(b, b, gcd);
    mpz_t m_reduced;
    mpz_init(m_reduced);
    mpz_fdiv_q(m_reduced, m, gcd);

    // Compute modular inverse of a (mod m/g) to find a^-1
    if (mpz_invert(inverse_a, a, m_reduced) == 0) {
        printf("No solution exists as inverse of %ld (mod %ld) does not exist!\n",
               mpz_get_ui(a), mpz_get_ui(m_reduced));

        // Free allocated memory and exit
        mpz_clears(a, b, m, gcd, inverse_a, solution, m_reduced, NULL);
        return 0;
    }

    // Compute initial solution x = (b * a^-1) mod (m/g)
    mpz_mul(solution, b, inverse_a);
    mpz_mod(solution, solution, m_reduced);

    // Print all solutions: x = x_0 + k * (m/g) for k = 0 to gcd-1
    mpz_t k, step_size, current_solution;
    mpz_inits(k, step_size, current_solution, NULL);
    mpz_set_ui(k, 0);
    mpz_set(step_size, m_reduced);
    mpz_set(current_solution, solution);

    printf("Solutions to %ldx ≡ %ld (mod %ld):\n", mpz_get_ui(a), mpz_get_ui(b), mpz_get_ui(m_reduced));
    
    while (mpz_cmp(k, gcd) < 0) {
        printf("x = %ld\n", mpz_get_ui(current_solution));

        // Compute next solution x = x + (m/g)
        mpz_add(current_solution, current_solution, step_size);
        mpz_add_ui(k, k, 1);
    }

    // Print number of solutions
    printf("Number of solutions: %ld\n", mpz_get_ui(gcd));

    // Free allocated memory
    mpz_clears(a, b, m, gcd, inverse_a, solution, m_reduced, k, step_size, current_solution, NULL);

    return 0;
}
