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

/**
 * Structure that holds a word along with its signature
 */
typedef struct {
    /** The original word */
    char *original;
    /** The word signature */
    char *signature;
    /** The length of the word */
    size_t length;
} SignaturePair;

/**
 * Comparison function for sorting an array of string signature elements.
 *
 * @param p Pointer to the first array element to compare.
 * @param q Pointer to the second array element to compare.
 * @return -1 if the first element is less that the second, 1 if the first element is greater then the second or 0 if
 * the two elements are equal.
 */
int compare_signature_pairs(const void *p, const void *q) {
    SignaturePair x = *(const SignaturePair*) p;
    SignaturePair y = *(const SignaturePair*) q;
    size_t max_length = (x.length < y.length) ? y.length : x.length;

    return strncmp(x.signature, y.signature, max_length);
}

/**
 * Write an anagram db entry to the output file.
 *
 * @param file The output file.
 * @param pairs The signature pairs.
 * @param first_entry The index of the first signature pair to write.
 * @param last_entry The index of the last signature pair to write.
 */
void write_db_entry(FILE *file, const SignaturePair *pairs, size_t first_entry, size_t last_entry) {
    fputc((char) pairs[first_entry].length, file);
    fputs(pairs[first_entry].signature, file);
    fputc((char) (last_entry - first_entry + 1), file);
    for (size_t j = first_entry; j <= last_entry; j++) {
        fputs(pairs[j].original, file);
    }
}

// The number of entries by which the database will be extended, if there is no space left
#define CHUNK_SIZE 1000

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
    if (argc < 3) {
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
    size_t n = 0;
    ssize_t line_length;
    SignaturePair *pairs = malloc(CHUNK_SIZE * sizeof (SignaturePair));
    size_t word_count = 0;
    size_t capacity = CHUNK_SIZE;
    while ((line_length = getline(&line, &n, input_file)) != -1) {
        // Strip new line if it exists
        if (line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
            line_length--;
        }

        // Crate the signature pair
        char *original = strndup(line, line_length);
        char *signature = malloc((line_length + 1) * sizeof (char));
        ss_calculate(original, line_length, signature);
        SignaturePair pair = {.signature = signature, .original = original, .length = line_length};
        pairs[word_count++] = pair;

        // Extend the capacity if needed
        if (word_count == capacity) {
            capacity += CHUNK_SIZE;
            pairs = realloc(pairs, capacity * sizeof(SignaturePair));
        }
    }

    // Sort the signature pairs
    qsort(pairs, word_count, sizeof (SignaturePair), compare_signature_pairs);

    // Build the database
    int exit_status = EXIT_SUCCESS;
    FILE *output_file = NULL;
    if (word_count > 0) { // Check if not empty
        // Open the output file
        output_file = fopen(argv[2], "wb");
        if (output_file == NULL) {
            exit_status = EXIT_FAILURE;
            fprintf(stderr, "Unable to open the output file %s for writing.\n", argv[1]);
            goto cleanup;
        }

        // Group entries with the same signature together
        size_t first_entry = 0;
        size_t last_entry = 0;
        for (size_t i = 1; i < word_count; i++) {
            if (pairs[i].length == pairs[first_entry].length &&
                strncmp(pairs[i].signature, pairs[first_entry].signature, pairs[first_entry].length) == 0) {
                // The signature is the same, continue with the next entry
                last_entry = i;
            } else {
                // Different signature. If there are more than one words with the same signature, write them to the
                // output file, as they are anagrams
                if (first_entry != last_entry) {
                    write_db_entry(output_file, pairs, first_entry, last_entry);
                }
                first_entry = i;
                last_entry = i;
            }
        }
        if (first_entry != last_entry) {
            write_db_entry(output_file, pairs, first_entry, last_entry);
        }
    }

    // Cleanup
cleanup:
    if (output_file != NULL) {
        fclose(output_file);
    }
    for (size_t i = 0; i < word_count; i++) {
        free(pairs[i].original);
        free(pairs[i].signature);
    }
    free(pairs);
    free(line);
    fclose(input_file);

    return exit_status;
}
