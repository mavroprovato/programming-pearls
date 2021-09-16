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
    StringSignature input;
    if (!ss_init(&input, argv[2])) {
        return EXIT_FAILURE;
    }

    // Read the dictionary
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1) {
        // Strip new line if it exists
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        if (ss_matches(line, &input)) {
            puts(line);
        }
    }

    ss_destroy(&input);
    free(line);
    fclose(file);

    return EXIT_SUCCESS;
}
