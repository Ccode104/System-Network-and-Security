#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>

void main(int argc, char* argv[]) {
    int num_inputs = argc - 1; // Number of integers (excluding the program name)
    mpz_t *numbers; // Array to store the input integers
    numbers = (mpz_t*)malloc(sizeof(mpz_t) * num_inputs);

    // Initialize GMP variables for each input number
    for (int i = 0; i < num_inputs; i++) {
        mpz_init(numbers[i]);
    }

    int max_possible_size = 10;

    int is_common_divisor; // Flag to check if the current divisor is common to all numbers
    mpz_t common_divisors[max_possible_size]; // Array to store common divisors

    // Initialize GMP variables for common divisors
    for (int i = 0; i < max_possible_size; i++) {
        mpz_init(common_divisors[i]);
    }

    mpz_set_ui(common_divisors[0], 1); // 1 is a trivial common divisor
    int common_divisors_count = 1; // Start with 1 as a common divisor

    mpz_t divisor;
    mpz_init(divisor);
    mpz_set_ui(divisor, 2); // Start checking divisors from 2

    // Store the input integers into the numbers array
    for (int i = 0; i < num_inputs; i++) {
        mpz_set_ui(numbers[i], atoi(argv[i + 1])); // Convert string to integer
    }

    // Initialize minimum number
    mpz_t min_value;
    mpz_init(min_value);
    mpz_set(min_value, numbers[0]);

    // Find the minimum number from the input
    for (int i = 0; i < num_inputs; i++) {
        if (mpz_get_ui(min_value) < mpz_get_ui(numbers[i])) {
            mpz_set(min_value, numbers[i]);
        }
    }


    // Check for common divisors
    while (mpz_cmp(divisor, min_value) <= 0) {
        is_common_divisor = 1; // Assume the current divisor is common

        // Check if the current divisor divides all the numbers
        for (int i = 0; i < num_inputs; i++) {
            if (mpz_divisible_p(numbers[i], divisor) == 0) {
                is_common_divisor = 0; // Not a common divisor
                break;
            }
        }

        // If the divisor is common, store it
        if (is_common_divisor == 1) {
            if (common_divisors_count < max_possible_size) {  // Prevent overflow
                mpz_set(common_divisors[common_divisors_count], divisor);
                common_divisors_count++;
            } else {
                printf("Error: Too many common divisors\n");
                break;
            }
        }

    // Increment the divisor
    mpz_add_ui(divisor, divisor, 1);
}


    // Print all common divisors
    printf("Common divisors are: ");
    for (int i = 0; i < common_divisors_count; i++) {
        printf("%ld ", mpz_get_ui(common_divisors[i]));
    }

    // Free the dynamically allocated memory
    for (int i = 0; i < num_inputs; i++) {
        mpz_clear(numbers[i]);
        mpz_clear(common_divisors[i]);
    }
    free(numbers);
    mpz_clear(divisor);
    mpz_clear(min_value);
}
