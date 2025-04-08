#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <stdlib.h>

void generate_random_number(mpz_t result, const mpz_t p) {
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));
    mpz_urandomm(result, state, p);
    gmp_randclear(state);
}

int main() {
    mpz_t p, g, x, y, R1, R2, K_alice, K_bob;
    
    mpz_inits(p, g, x, y, R1, R2, K_alice, K_bob, NULL);
    
    // Define a large prime number p and generator g (example values, should be securely generated in real applications)
    mpz_set_str(p, "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E08" 
                    "8A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B30" 
                    "2B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A63A" 
                    "3620FFFFFFFFFFFFFFFF", 16);
    mpz_set_ui(g, 2); // Commonly used generator value
    
    // Alice generates her secret key x and computes R1 = g^x mod p
    generate_random_number(x, p);
    mpz_powm(R1, g, x, p);
    
    // Bob generates his secret key y and computes R2 = g^y mod p
    generate_random_number(y, p);
    mpz_powm(R2, g, y, p);
    
    // Exchange R1 and R2
    printf("Public values:\n");
    gmp_printf("p = %Zd\n", p);
    gmp_printf("g = %Zd\n", g);
    
    printf("\nExchanged values:\n");
    gmp_printf("Alice sends R1 = %Zd\n", R1);
    gmp_printf("Bob sends R2 = %Zd\n", R2);
    
    // Compute the shared secret key K
    mpz_powm(K_alice, R2, x, p); // Alice computes K = R2^x mod p
    mpz_powm(K_bob, R1, y, p);   // Bob computes K = R1^y mod p
    
    printf("\nComputed shared secret key:\n");
    gmp_printf("Alice's computed key: %Zd\n", K_alice);
    gmp_printf("Bob's computed key: %Zd\n", K_bob);
    
    // Verify both computed keys match
    if (mpz_cmp(K_alice, K_bob) == 0) {
        printf("\nKey exchange successful! Both parties have the same shared key.\n");
    } else {
        printf("\nKey exchange failed! Keys do not match.\n");
    }
    
    // Clear memory
    mpz_clears(p, g, x, y, R1, R2, K_alice, K_bob, NULL);
    return 0;
}