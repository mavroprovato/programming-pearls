/**
 * This program searches for anagrams for the input word, using the preprocessed anagram database.
 *
 * This is a solution for problem 1.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stringsig.h"

// The number of entries by which the database will be extended, if there is no space left
#define CHUNK_SIZE 1000

/**
 * The anagram database entry.
 */
typedef struct {
    /** The length of the words of the entry */
    uint8_t length;
    /** The entry signature */
    char *signature;
    /** The number of anagrams */
    uint8_t word_count;
    /** The anagrams */
    char **words;
} Entry;

/**
 * Read a database entry from the file.
 *
 * @param file The file to read.
 * @param entry The entry to read.
 * @return true if the entry was read successfully, false otherwise.
 */
bool read_db_entry(FILE *file, Entry *entry) {
    int length = fgetc(file);
    if (length == EOF) {
        return false;
    }
    entry->length = length;
    entry->signature = malloc((entry->length + 1) * sizeof (char));
    fgets(entry->signature, entry->length + 1, file);
    entry->word_count = fgetc(file);
    entry->words = malloc(sizeof (char *) * entry->word_count);
    for (size_t i = 0; i < entry->word_count; i++) {
        entry->words[i] = malloc((entry->length + 1) * sizeof (char));
        fgets(entry->words[i], entry->length + 1, file);
    }

    return true;
}

/**
 * Search a sorted array of entries for the signature.
 *
 * @param p The signature to find.
 * @param q The database entry to compare.
 * @return -1 if the signature is less that the entry, 1 if the signature is greater then entry or 0 if the two elements
 * are equal.
 */
int compare_entries(const void *p, const void *q) {
    char *ep = (char *) p;
    Entry *eq = (Entry *) q;

    return strcmp(ep, eq->signature);
}

/**
 * The main entry point of the program. It takes 2 required command line arguments: The anagram database file and the
 * word to search for. If anagrams are found, they are printed to the standard output.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return The program exit status.
 */
int main(int argc, char *argv[]) {
    // Validate the number of command line arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: search_anagram_db: [ANAGRAM_DB] [WORD]\n"
                        "Search the anagram database file [ANAGRAM_DB] for anagrams of [WORD], and print them to the "
                        "standard output.\n");
        return EXIT_FAILURE;
    }

    // Open the database file
    FILE *input_file = fopen(argv[1], "rb");
    if (input_file == NULL) {
        fprintf(stderr, "Unable to open database file %s.\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Read the database
    Entry entry;
    size_t entry_count = 0;
    size_t current_size = CHUNK_SIZE;
    Entry *entries = malloc(current_size * sizeof(Entry));
    while (read_db_entry(input_file, &entry)) {
        entries[entry_count++] = entry;
        if (entry_count == current_size) {
            current_size += CHUNK_SIZE;
            entries = realloc(entries, current_size * sizeof(Entry));
        }
    }

    // Calculate the signature of the input word
    size_t signature_length = strlen(argv[2]);
    char *signature = malloc((signature_length + 1) * sizeof(char));
    ss_calculate(argv[2], signature_length, signature);

    // Search the database for signature
    Entry *matched_entry = bsearch(signature, entries, entry_count, sizeof (Entry), compare_entries);
    if (matched_entry != NULL) {
        for (size_t i = 0; i < matched_entry->word_count; i++) {
            if (strncmp(matched_entry->words[i], argv[2], matched_entry->length) != 0) {
                puts(matched_entry->words[i]);
            }
        }
    }

    // Cleanup
    for (size_t i = 0; i < entry_count; i++) {
        free(entries[i].signature);
        for (size_t j = 0; j < entries[i].word_count; j++) {
            free(entries[i].words[j]);
        }
        free(entries[i].words);
    }
    free(entries);
    free(signature);
    fclose(input_file);

    return EXIT_SUCCESS;
}