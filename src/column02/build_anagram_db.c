/**
 * This program builds a anagram database from a dictionary. The database maps the dictionary words to a signature,
 * which is the word with all its letters sorted. Words with the same signature are anagrams of each other.
 *
 * This is a solution for problem 1.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stringsig.h"

// The number of entries by which the database will be extended, if there is no space left
#define CHUNK_SIZE 1000

/**
 * Structure for the database entry.
 */
typedef struct {
    StringSignature *signature;
    char *original;
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
    Entry *ep = (Entry *) p;
    Entry *eq = (Entry *) q;

    return strcmp(ep->signature->signature, eq->signature->signature);
}

/**
 * The main entry point of the program. It takes 2 required command line arguments: The dictionary file and the output
 * file where the anagram database will be written.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return The program exit status.
 */
int main(int argc, char *argv[]) {
    // Validate the number of command line arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: build_anagram_db: [DICTIONARY] [OUTPUT]\n"
                        "Build an anagram database from the [DICTIONARY] file and write it to the [OUTPUT] file.\n");
        return EXIT_FAILURE;
    }

    // Open the input input_file
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        fprintf(stderr, "Unable to open the dictionary file %s.\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Read the dictionary
    char *line = NULL;
    size_t len = 0;
    size_t current_entry = 0;
    size_t current_size = CHUNK_SIZE;
    Entry *entries = malloc(current_size * sizeof(Entry));
    while (getline(&line, &len, input_file) != -1) {
        // Strip new line if it exists
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        // Create the entry
        StringSignature *signature = malloc(sizeof(StringSignature));
        ss_init(signature, line);
        Entry entry = {.signature = signature, .original = strdup(line)};
        entries[current_entry] = entry;

        // Allocate more space if needed
        current_entry++;
        if (current_entry == current_size) {
            current_size += CHUNK_SIZE;
            entries = realloc(entries, current_size * sizeof(Entry));
        }
    }

    int exit_status = EXIT_SUCCESS;
    FILE *output_file = NULL;
    if (current_entry > 0) { // Check if not empty
        // Open the output file
        output_file = fopen(argv[2], "wb");
        if (output_file == NULL) {
            exit_status = EXIT_FAILURE;
            fprintf(stderr, "Unable to open the output file %s for writing.\n", argv[1]);
            goto cleanup;
        }
        // Sort the entries by signature
        qsort(entries, current_entry, sizeof (Entry), compare_entries);
        // Group entries with the same signature together
        size_t first_entry = 0;
        size_t last_entry = 0;
        for (size_t i = 1; i < current_entry; i++) {
            if (entries[i].signature->n == entries[first_entry].signature->n &&
                strcmp(entries[i].signature->signature, entries[first_entry].signature->signature) == 0) {
                // Same signature
                last_entry = i;
            } else {
                // Different signature. If there are more than one words with the same signature, write them to the
                // output file, as they are anagrams
                if (first_entry != last_entry) {
                    fputc((char) entries[first_entry].signature->n, output_file);
                    fprintf(output_file, "%s", entries[first_entry].signature->signature);
                    fputc((char) (last_entry - first_entry + 1), output_file);
                    for (size_t j = first_entry; j <= last_entry; j++) {
                        fprintf(output_file, "%s", entries[j].original);
                    }
                }
                first_entry = i;
                last_entry = i;
            }
        }
    }

    // Cleanup
cleanup:
    fclose(input_file);
    if (output_file != NULL) {
        fclose(output_file);
    }
    free(line);
    for (size_t i = 0; i < current_entry; i++) {
        ss_destroy(entries[i].signature);
        free(entries[i].signature);
        free(entries[i].original);
    }
    free(entries);

    return exit_status;
}
