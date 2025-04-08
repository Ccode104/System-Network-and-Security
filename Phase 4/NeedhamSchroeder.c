#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define KEY_SIZE 256

void generate_nonce(mpz_t nonce) {
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));
    mpz_urandomb(nonce, state, KEY_SIZE);
    gmp_randclear(state);
}

void encrypt(mpz_t ciphertext, mpz_t plaintext, mpz_t key) {
    mpz_powm(ciphertext, plaintext, key, key);  // Simulating secure encryption using exponentiation
}

void decrypt(mpz_t plaintext, mpz_t ciphertext, mpz_t key) {
    mpz_powm(plaintext, ciphertext, key, key);  // Simulating decryption (same as encryption for demonstration)
}

int main() {
    mpz_t RA, RB, session_key, alice_key, bob_key, alice_identity, bob_identity;
    mpz_t ticket, encrypted_message, decrypted_message, encrypted_ticket, full_message;
    
    mpz_inits(RA, RB, session_key, alice_key, bob_key, alice_identity, bob_identity, ticket, encrypted_message, decrypted_message, encrypted_ticket, full_message, NULL);
    
    // Simulating secret keys for Alice and Bob
    mpz_set_ui(alice_key, 12345);
    mpz_set_ui(bob_key, 67890);
    mpz_set_ui(alice_identity, 1); // Assign a numerical ID to Alice for encryption
    mpz_set_ui(bob_identity, 2); // Assign a numerical ID to Bob for encryption
    
    // Step 1: Alice sends a request to KDC with her identity and Bob's identity
    generate_nonce(RA);
    gmp_printf("Alice generates nonce RA: %Zd\n", RA);
    char alice_id[] = "Alice";
    char bob_id[] = "Bob";
    gmp_printf("Alice sends message to KDC: [RA: %Zd, Alice_ID: %s, Bob_ID: %s]\n", RA, alice_id, bob_id);
    
    // Step 2: KDC generates session key and encrypts the full message with Alice's secret key
    generate_nonce(session_key);
    gmp_printf("KDC generates session key: %Zd\n", session_key);
    
    // Creating ticket that includes Alice's identity, session key, and Bob's identity
    mpz_xor(ticket, alice_identity, session_key);
    mpz_xor(ticket, ticket, bob_identity);
    encrypt(ticket, ticket, bob_key);  // Encrypt ticket using Bob's secret key
    
    // Combine RA, Bob_ID, session_key, and ticket into a single message
    mpz_xor(full_message, RA, session_key);
    mpz_xor(full_message, full_message, ticket);
    mpz_xor(full_message, full_message, bob_identity);
    
    encrypt(encrypted_message, full_message, alice_key);  // Encrypt the full message using Alice's key
    
    gmp_printf("KDC sends encrypted message to Alice: %Zd\n", encrypted_message);
    
    // Step 3: Alice decrypts and extracts session key
    decrypt(decrypted_message, encrypted_message, alice_key);
    mpz_xor(session_key, decrypted_message, RA);
    mpz_xor(session_key, session_key, ticket);
    mpz_xor(session_key, session_key, bob_identity);
    gmp_printf("Alice decrypts and extracts session key: %Zd\n", session_key);
    
    // Alice forwards ticket to Bob
    gmp_printf("Alice sends encrypted ticket to Bob: %Zd\n", ticket);
    
    // Step 4: Bob decrypts ticket and sends challenge to Alice
    decrypt(decrypted_message, ticket, bob_key);
    mpz_xor(session_key, decrypted_message, alice_identity);
    mpz_xor(session_key, session_key, bob_identity);
    gmp_printf("Bob extracts session key: %Zd\n", session_key);
    generate_nonce(RB);
    gmp_printf("Bob generates nonce RB: %Zd\n", RB);
    
    encrypt(encrypted_message, RB, session_key);
    gmp_printf("Bob sends encrypted challenge: %Zd\n", encrypted_message);
    
    // Step 5: Alice decrypts RB and responds with RB - 1
    decrypt(decrypted_message, encrypted_message, session_key);
    mpz_sub_ui(decrypted_message, decrypted_message, 1);
    gmp_printf("Alice responds with RB-1: %Zd\n", decrypted_message);
    
    // Clean up
    mpz_clears(RA, RB, session_key, alice_key, bob_key, alice_identity, bob_identity, ticket, encrypted_message, decrypted_message, encrypted_ticket, full_message, NULL);
    return 0;
}
