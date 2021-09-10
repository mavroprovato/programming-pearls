/**
 * This program checks if a word is an anagram of a word found in a dictionary. No preprocessing is performed.
 *
 * This is a solution for problem 1.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Comparison function for sorting an array of char elements.
 *
 * @param p Pointer to the first array element to compare.
 * @param q Pointer to the second array element to compare.
 * @return -1 if the first element is less that the second, 1 if the first element is greater then the second or 0 if
 * the two elements are equal.
 */
int compare_char(const void *p, const void *q) {
    char x = *(const char*) p;
    char y = *(const char*) q;

    if (x < y) {
        return -1;
    } else if (x > y) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Check if a string has the provided signature. The signature of a string is a string with the characters sorted.
 *
 * @param input The input string to check.
 * @param signature The signature string. It must be sorted.
 * @param signature_len The signature string length.
 * @return True if the signature matches, false otherwise.
 */
bool signature_matches(char *input, char *signature, size_t signature_len) {
    size_t input_len = strlen(input);
    if (input_len != signature_len) {
        return false;
    }

    char *input_sorted = strdup(input);
    qsort(input_sorted, strlen(input_sorted), sizeof(char), compare_char);
    bool anagram = strcmp(input_sorted, signature) == 0;
    free(input_sorted);

    return anagram;
}

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

    // Sort & get the length of the input word
    char *signature = argv[2];
    qsort(signature, strlen(signature), sizeof(char), compare_char);
    size_t signature_len = strlen(signature);

    // Read the dictionary
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1) {
        // Strip new line if it exists
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }
        if (signature_matches(line, signature, signature_len)) {
            puts(line);
        }
    }

    fclose(file);
    free(line);

    return EXIT_SUCCESS;
}


