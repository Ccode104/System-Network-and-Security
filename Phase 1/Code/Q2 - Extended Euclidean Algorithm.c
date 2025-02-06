#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>

void main(int argc, char *argv[]) {
    // Initialize the input numbers a and b
    mpz_t a, b;
    mpz_init(a);
    mpz_init(b);

    // Set values for a and b from the command-line arguments
    mpz_set_si(a, atoi(argv[1]));
    mpz_set_si(b, atoi(argv[2]));

    // Initialize variables for the Extended Euclidean Algorithm
    mpz_t r1, r2, r, q, t1, t2, t, s1, s2, s;
    mpz_inits(r1, r2, r, q, t1, t2, t, s1, s2, s, NULL);

    // Set initial values
    mpz_set(r1, a); // r1 = a
    mpz_set(r2, b); // r2 = b
    mpz_set_si(s1, 1); // s1 = 1
    mpz_set_si(s2, 0); // s2 = 0
    mpz_set_si(t1, 0); // t1 = 0
    mpz_set_si(t2, 1); // t2 = 1

    // Print the headers for the algorithm's steps
    printf("q       r1      r2      r       s1      s2      s       t1      t2      t\n");

    // Perform the Extended Euclidean Algorithm
    while (mpz_cmp_si(r2,0) ){
        

        // Perform the division and update the remainders
        mpz_fdiv_qr(q, r, r1, r2);  // q = r1 / r2

        // Update r1 and r2 for the next iteration
        mpz_set(r1, r2);
        mpz_set(r2, r);

        // Update the coefficients for x (s) and y (t)
        mpz_mul(t, t2, q);      // t = t2 * q
        mpz_sub(t, t1, t);      // t = t1 - t

        // Update t1 and t2 for the next iteration
        mpz_set(t1, t2);
        mpz_set(t2, t);

        mpz_mul(s, s2, q);      // s = s2 * q
        mpz_sub(s, s1, s);      // s = s1 - s

        // Update s1 and s2 for the next iteration
        mpz_set(s1, s2);
        mpz_set(s2, s);

        // Print the current step values
    printf("%ld      %ld      %ld      %ld      %ld      %ld      %ld      %ld      %ld      %ld\n", 
            mpz_get_si(q), 
            mpz_get_si(r1), 
            mpz_get_si(r2), 
            mpz_get_si(r), 
            mpz_get_si(s1), 
            mpz_get_si(s2), 
            mpz_get_si(s), 
            mpz_get_si(t1), 
            mpz_get_si(t2), 
            mpz_get_si(t));
    }

    // Print the final result
    printf("GCD is: %ld, x = %ld, y = %ld\n", 
        mpz_get_si(r1), // The GCD
        mpz_get_si(s1), // x
        mpz_get_si(t1)); // y
}
