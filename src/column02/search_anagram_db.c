/**
 * This program searches for anagrams for the input word, using the preprocessed anagram database.
 *
 * This is a solution for problem 1.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stringsig.h>

// The max anagram length
#define MAX_ANAGRAM_LENGTH 265
// The number of entries by which the database will be extended, if there is no space left
#define CHUNK_SIZE 1000

typedef struct {
    char *signature;
    uint8_t length;
    uint8_t anagram_count;
    char **words;
} Entry;

/**
 * Compares two database entries. The entries are compared based on the dictionary word's signature.
 *
 * @param p The first database entry to compare.
 * @param q The second database entry to compare.
 * @return -1 if the first element is less that the second, 1 if the first element is greater then the second or 0 if
 * the two elements are equal.
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
    if (argc < 2) {
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

    // Read the anagram database
    char buffer[MAX_ANAGRAM_LENGTH] = {0};
    int anagram_length;
    size_t current_entry = 0;
    size_t current_size = CHUNK_SIZE;
    Entry *entries = malloc(current_size * sizeof(Entry));
    while ((anagram_length = fgetc(input_file)) != EOF) {
        Entry entry = {.length = anagram_length};
        // Read the signature
        fgets(buffer, anagram_length + 1, input_file);
        entry.signature = strdup(buffer);
        // Read the anagrams
        entry.anagram_count= fgetc(input_file);
        entry.words = malloc(sizeof (char *) * entry.anagram_count);
        for (size_t i = 0; i < entry.anagram_count; i++) {
            fgets(buffer, anagram_length + 1, input_file);
            entry.words[i] = strdup(buffer);
        }
        entries[current_entry++] = entry;
        if (current_entry == current_size) {
            current_size += CHUNK_SIZE;
            entries = realloc(entries, current_size * sizeof(Entry));
        }
    }

    // Calculate the signature of the input word
    StringSignature signature;
    ss_init(&signature, argv[2]);

    // Search for the signature
    Entry *matched_entry = bsearch(signature.signature, entries, current_entry, sizeof (Entry), compare_entries);
    if (matched_entry != NULL) {
        for (size_t i = 0; i < matched_entry->anagram_count; i++) {
            printf("%s\n", matched_entry->words[i]);
        }
    }

    // Cleanup
    fclose(input_file);
    for (size_t i = 0; i < current_entry; i++) {
        free(entries[i].signature);
        for (size_t j = 0; j < entries[i].anagram_count; j++) {
            free(entries[i].words[j]);
        }
        free(entries[i].words);
    }
    free(entries);
    ss_destroy(&signature);

    return EXIT_SUCCESS;
}