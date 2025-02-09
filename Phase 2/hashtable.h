// hashtable.h

#ifndef HASHTABLE_H
#define HASHTABLE_H

// Define the structure of the hash table entry
typedef struct Entry {
    char key;  // Key can be a digit, lowercase letter, or space
    char value;  // Store a string or any value associated with the key
} Entry;

// Hashtable definition
#define TABLE_SIZE 37

extern Entry table[TABLE_SIZE];

// Hash function to map a key to an index
unsigned int hash(char key);

// Function to insert a key-value pair into the hashtable
void insert(char key, const char value);

// Function to search for a key and return the associated value
char search(char key);

// Function to delete a key-value pair from the hashtable
void delete(char key);

// Function to check if a key is present in the hashtable
int contains_key(char key);

// Function to print the hashtable (for debugging)
void print_table();

#endif // HASHTABLE_H
