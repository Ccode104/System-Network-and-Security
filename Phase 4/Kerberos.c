#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include <time.h>

// Function to generate a random session key
void generate_session_key(mpz_t key) {
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, (unsigned long)clock());
    mpz_urandomb(key, state, 256); // Generate a 256-bit session key
    gmp_randclear(state);
}

// Convert a string (identity) to an integer representation
void string_to_mpz(mpz_t result, const char *str) {
    mpz_set_ui(result, 0);
    for (size_t i = 0; i < strlen(str); i++) {
        mpz_mul_ui(result, result, 256); // Shift left by 8 bits (1 byte per char)
        mpz_add_ui(result, result, (unsigned char)str[i]);
    }
}

// Function to encrypt a message using XOR
void encrypt(mpz_t result, mpz_t message, mpz_t key) {
    mpz_xor(result, message, key);
}

// Function to decrypt a message (XOR decryption is same as encryption)
void decrypt(mpz_t result, mpz_t encrypted, mpz_t key) {
    mpz_xor(result, encrypted, key);
}

int main() {
    mpz_t KA_AS, KA_TGS, KAS_TGS, Ticket_TGS;
    mpz_t KA_B, KB_TGS, Ticket_AB_Alice, Ticket_AB_Bob;
    mpz_t Alice_ID, Bob_ID, Timestamp, Encrypted_Timestamp, Decrypted_Timestamp, Encrypted_Confirmation;

    mpz_inits(KA_AS, KA_TGS, KAS_TGS, Ticket_TGS, KA_B, KB_TGS,
              Ticket_AB_Alice, Ticket_AB_Bob, Alice_ID, Bob_ID,
              Timestamp, Encrypted_Timestamp, Decrypted_Timestamp, Encrypted_Confirmation, NULL);

    // Define Alice and Bob's identity
    const char *AliceStr = "Alice";
    const char *BobStr = "Bob";

    // Convert IDs to integer format
    string_to_mpz(Alice_ID, AliceStr);
    string_to_mpz(Bob_ID, BobStr);

    printf("\n---- Step 1: Alice requests authentication from AS ----\n");
    printf("[Sender: Alice] --> [Receiver: AS]\n");
    printf("Message: Authentication request containing Alice's ID\n");
    gmp_printf("Alice ID: %Zd\n\n", Alice_ID);

    printf("\n---- Step 2: AS generates session key and ticket ----\n");
    generate_session_key(KA_TGS);
    generate_session_key(KAS_TGS);

    // Encrypt Ticket_TGS (contains KA-TGS and Alice ID) with KA-AS
    mpz_mul_2exp(Ticket_TGS, KA_TGS, 512);
    mpz_add(Ticket_TGS, Ticket_TGS, Alice_ID);
    encrypt(Ticket_TGS, Ticket_TGS, KAS_TGS);
    
    printf("[Sender: AS] --> [Receiver: Alice]\n");
    printf("Message: Encrypted session key (KA-TGS) and Ticket for TGS\n");
    gmp_printf("KA_TGS: %Zd\n", KA_TGS);
    gmp_printf("Encrypted Ticket_TGS: %Zd\n\n", Ticket_TGS);

    printf("\n---- Step 3: Alice decrypts Ticket_TGS and contacts TGS ----\n");
    decrypt(Ticket_TGS, Ticket_TGS, KAS_TGS);

    // Extract KA-TGS from Ticket_TGS
    mpz_t Extracted_KA_TGS;
    mpz_init(Extracted_KA_TGS);
    mpz_fdiv_q_2exp(Extracted_KA_TGS, Ticket_TGS, 512);

    gmp_printf("Alice extracts KA_TGS: %Zd\n", Extracted_KA_TGS);

    // Generate timestamp
    mpz_set_ui(Timestamp, time(NULL));
    encrypt(Encrypted_Timestamp, Timestamp, KA_TGS);

    printf("[Sender: Alice] --> [Receiver: TGS]\n");
    printf("Message: Ticket_TGS, timestamp (encrypted), and Bob ID\n");
    gmp_printf("Encrypted Timestamp: %Zd\n", Encrypted_Timestamp);
    gmp_printf("Bob ID: %Zd\n\n", Bob_ID);

    printf("\n---- Step 4: TGS generates session key for Alice and Bob ----\n");
    generate_session_key(KA_B);
    generate_session_key(KB_TGS);

    // Create Ticket_AB for Bob: (KA-B || Alice_ID) encrypted with KB-TGS
    mpz_mul_2exp(Ticket_AB_Bob, KA_B, 512);
    mpz_add(Ticket_AB_Bob, Ticket_AB_Bob, Alice_ID);
    encrypt(Ticket_AB_Bob, Ticket_AB_Bob, KB_TGS);

    // Create Ticket_AB for Alice: (KA-B || Bob_ID) encrypted with KA-TGS
    mpz_mul_2exp(Ticket_AB_Alice, KA_B, 512);
    mpz_add(Ticket_AB_Alice, Ticket_AB_Alice, Bob_ID);
    encrypt(Ticket_AB_Alice, Ticket_AB_Alice, KA_TGS);

    printf("[Sender: TGS] --> [Receiver: Alice]\n");
    printf("Message: Two encrypted tickets (one for Alice and one for Bob)\n");
    gmp_printf("Encrypted Ticket_AB for Bob: %Zd\n", Ticket_AB_Bob);
    gmp_printf("Encrypted Ticket_AB for Alice: %Zd\n\n", Ticket_AB_Alice);

    printf("\n---- Step 5: Alice forwards Ticket_AB and timestamp to Bob ----\n");
    encrypt(Encrypted_Timestamp, Timestamp, KA_B);

    printf("[Sender: Alice] --> [Receiver: Bob]\n");
    printf("Message: Ticket_AB_Bob (unchanged) and encrypted timestamp\n");
    gmp_printf("Encrypted Timestamp: %Zd\n", Encrypted_Timestamp);
    gmp_printf("Ticket_AB_Bob: %Zd\n\n", Ticket_AB_Bob);

    printf("\n---- Step 6: Bob verifies Alice and replies ----\n");

    // Bob decrypts Ticket_AB_Bob using KB-TGS to get KA-B
    decrypt(Ticket_AB_Bob, Ticket_AB_Bob, KB_TGS);
    
    // Extract KA-B
    mpz_t Extracted_KA_B;
    mpz_init(Extracted_KA_B);
    mpz_fdiv_q_2exp(Extracted_KA_B, Ticket_AB_Bob, 512);

    // Bob decrypts timestamp using KA-B
    decrypt(Decrypted_Timestamp, Encrypted_Timestamp, Extracted_KA_B);
    gmp_printf("Bob decrypts Timestamp: %Zd\n", Decrypted_Timestamp);

    // Bob modifies timestamp (decrement by 1)
    mpz_sub_ui(Decrypted_Timestamp, Decrypted_Timestamp, 1);

    // Encrypt the modified timestamp using KA-B
    encrypt(Encrypted_Confirmation, Decrypted_Timestamp, Extracted_KA_B);

    printf("[Sender: Bob] --> [Receiver: Alice]\n");
    printf("Message: Encrypted timestamp confirmation (decremented by 1)\n");
    gmp_printf("Encrypted Confirmation: %Zd\n", Encrypted_Confirmation);

    // Cleanup
    mpz_clears(KA_AS, KA_TGS, KAS_TGS, Ticket_TGS, KA_B, KB_TGS,
               Ticket_AB_Alice, Ticket_AB_Bob, Alice_ID, Bob_ID,
               Timestamp, Encrypted_Timestamp, Decrypted_Timestamp, Encrypted_Confirmation,
               Extracted_KA_TGS, Extracted_KA_B, NULL);

    return 0;
}
