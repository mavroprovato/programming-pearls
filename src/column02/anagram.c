/**
 * This program checks if a word is an anagram of a word found in a dictionary. No preprocessing is performed.
 *
 * This is a solution for problem 1.
 */
#include "stringsig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * The main entry point of the program. It takes 2 required command line arguments: The dictionary file and the word we
 * want to search the anagram for.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return The program exit status.
 */
int main(int argc, char *argv[]) {
    // Validate the number of command line arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: anagram: [DICTIONARY] [WORD]\n"
                        "Find all anagrams of [WORD] in the [DICTIONARY].\n");
        return EXIT_FAILURE;
    }

    // Open the input file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open input file %s.\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Calculate the signature of the input
    size_t signature_length = strlen(argv[2]);
    char signature[signature_length + 1];
    ss_calculate(argv[2], signature_length, signature);
    // Buffer to hold the target signature
    char target_signature[signature_length + 1];
    // Read the dictionary
    char *line = NULL;
    size_t n = 0;
    ssize_t line_length;
    while ((line_length = getline(&line, &n, file)) != -1) {
        // Strip new line if it exists
        if (line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
            line_length--;
        }
        // Check if signatures match
        if (signature_length == line_length) {
            ss_calculate(line, line_length, target_signature);
            if (strncmp(signature, target_signature, signature_length) == 0 &&
                strncmp(argv[2], line, signature_length) != 0) {
                puts(line);
            }
        }
    }

    free(line);
    fclose(file);

    return EXIT_SUCCESS;
}
