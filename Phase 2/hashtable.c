// hashtable.c
#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialize the table globally
Entry table[TABLE_SIZE];

// Hash function to map a key to an index
unsigned int hash(char key) {
    if (key >= '0' && key <= '9') {
        return key - '0';  // Keys '0' to '9' map to indices 0-9
    } else if (key == ' ') {
        return 10;  // Space maps to index 10
    } else if (key >= 'a' && key <= 'z') {
        return 11 + (key - 'a');  // Keys 'a' to 'z' map to indices 11-36
    }
    return -1;  // Invalid key
}

// Function to insert a key-value pair into the hashtable
void insert(char key, const char value) {
    unsigned int index = hash(key);
    if (index == -1) {
        printf("Invalid key!\n");
        return;
    }

    // Directly store the value, overwriting if key already exists
    table[index].key = key;
    table[index].value = value;
}

// Function to search for a key and return the associated value
char search(char key) {
    unsigned int index = hash(key);
    if (index == -1) {
        return "Invalid key!";
    }

    // Check if the key matches
    if (table[index].key == key) {
        return table[index].value;
    }

    return "Key not found!";
}

// Function to delete a key-value pair from the hashtable
void delete(char key) {
    unsigned int index = hash(key);
    if (index == -1) {
        printf("Invalid key!\n");
        return;
    }

    // If key is found, reset the entry to "empty"
    if (table[index].key == key) {
        table[index].key = 0;  // Reset key to indicate it's not in use
        table[index].value = '\0';  // Clear the value
    } else {
        printf("Key not found!\n");
    }
}

// Function to check if a key is present in the hashtable
int contains_key(char key) {
    unsigned int index = hash(key);
    if (index == -1) {
        return 0;  // Invalid key
    }

    // Check if the key is present at the hashed index
    if (table[index].key == key) {
        return 1;  // Key found
    }

    return 0;  // Key not found
}

// Function to print the hashtable (for debugging)
void print_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table[i].key != 0) {  // Print only entries with a valid key
            printf("Index %d: ('%c' => %c)\n", i, table[i].key, table[i].value);
        }
    }
}
