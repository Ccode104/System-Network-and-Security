#include <stdio.h>
#include <gmp.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>

// Structure to hold the digital certificate fields
typedef struct {
    int version;
    mpz_t serial_number;
    char signature_algorithm[50];
    char issuer_name[100];
    time_t not_before;
    time_t not_after;
    char subject_name[100];
    mpz_t subject_public_key;
    char issuer_unique_id[50];
    char subject_unique_id[50];
    char extensions[200];
    mpz_t signature;
} Certificate;

// Function to generate a random big number
void generate_random(mpz_t value) {
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));
    mpz_urandomb(value, state, 256);
    gmp_randclear(state);
}

// Function to hash the certificate fields using SHA-256
void hash_certificate(mpz_t hash, const Certificate *cert) {
    char buffer[2000] = "";
    char temp[500];
    
    sprintf(temp, "%d", cert->version);
    strcat(buffer, temp);
    sprintf(temp, "%ld%ld", cert->not_before, cert->not_after);
    strcat(buffer, temp);
    strcat(buffer, cert->issuer_name);
    strcat(buffer, cert->subject_name);
    strcat(buffer, cert->signature_algorithm);
    strcat(buffer, cert->issuer_unique_id);
    strcat(buffer, cert->subject_unique_id);
    strcat(buffer, cert->extensions);
    
    unsigned char hash_output[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)buffer, strlen(buffer), hash_output);
    
    mpz_import(hash, SHA256_DIGEST_LENGTH, 1, 1, 1, 0, hash_output);
}

// Function to sign the certificate (simplified RSA signing)
void sign_certificate(mpz_t signature, const Certificate *cert, mpz_t private_key, mpz_t modulus) {
    mpz_t hash;
    mpz_init(hash);
    hash_certificate(hash, cert);
    mpz_powm(signature, hash, private_key, modulus);
    mpz_clear(hash);
}

// Function to verify the certificate signature
int verify_signature(mpz_t signature, const Certificate *cert, mpz_t public_key, mpz_t modulus) {
    mpz_t computed, hash;
    mpz_inits(computed, hash, NULL);
    mpz_powm(computed, signature, public_key, modulus);
    hash_certificate(hash, cert);
    int result = mpz_cmp(computed, hash) == 0;
    mpz_clears(computed, hash, NULL);
    return result;
}

// Function to initialize a certificate
void initialize_certificate(Certificate *cert) {
    cert->version = 2;
    mpz_init(cert->serial_number);
    generate_random(cert->serial_number);
    strcpy(cert->signature_algorithm, "RSA");
    strcpy(cert->issuer_name, "CertAuthority");
    cert->not_before = time(NULL);
    cert->not_after = cert->not_before + 31536000; // 1 year validity
    strcpy(cert->subject_name, "Alice");
    mpz_init(cert->subject_public_key);
    generate_random(cert->subject_public_key);
    strcpy(cert->issuer_unique_id, "Issuer123");
    strcpy(cert->subject_unique_id, "Subject456");
    strcpy(cert->extensions, "Extended Key Usage, Key Constraints");
    mpz_init(cert->signature);
}

// Function to print certificate details
void print_certificate(const Certificate *cert) {
    printf("Digital Certificate Details:\n");
    printf("Version: %d\n", cert->version);
    gmp_printf("Serial Number: %Zd\n", cert->serial_number);
    printf("Signature Algorithm: %s\n", cert->signature_algorithm);
    printf("Issuer: %s\n", cert->issuer_name);
    printf("Valid From: %s", ctime(&cert->not_before));
    printf("Valid Until: %s", ctime(&cert->not_after));
    printf("Subject: %s\n", cert->subject_name);
    gmp_printf("Subject Public Key: %Zd\n", cert->subject_public_key);
    printf("Issuer Unique ID: %s\n", cert->issuer_unique_id);
    printf("Subject Unique ID: %s\n", cert->subject_unique_id);
    printf("Extensions: %s\n", cert->extensions);
    gmp_printf("Signature: %Zd\n", cert->signature);
}

int main() {
    Certificate cert;
    initialize_certificate(&cert);

    // RSA private and public keys (simplified for demonstration)
    mpz_t private_key, public_key, modulus;
    mpz_inits(private_key, public_key, modulus, NULL);
    generate_random(private_key);
    generate_random(public_key);
    generate_random(modulus);

    // Signing the certificate
    sign_certificate(cert.signature, &cert, private_key, modulus);

    // Print certificate details
    print_certificate(&cert);

    // Verifying the certificate signature
    if (verify_signature(cert.signature, &cert, public_key, modulus)) {
        printf("Certificate Signature Verified Successfully!\n");
    } else {
        printf("Certificate Signature Verification Failed!\n");
    }

    // Cleanup
    mpz_clears(cert.serial_number, cert.subject_public_key, cert.signature, private_key, public_key, modulus, NULL);
    return 0;
}
