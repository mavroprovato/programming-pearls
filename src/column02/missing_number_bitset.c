/**
 * This program finds a missing number from an input file that contains at most 2^N - 1 N-bit unsigned integers in
 * random order. N is defined in compile time and should be either 8, 16, 32 or 64. The solution uses a bitset, so it
 * should only be used if there is ample amount of memory.
 *
 * This is a solution for problem A.
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bitset.h"

#define N 32
#define MAX_VALUE UINT32_MAX

/**
 * The main entry point of the program. It takes 1 required command line argument, which is the input file that contains
 * the integers.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return The program exit status.
 */
int main(int argc, char *argv[]) {
    // Validate the number of command line arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: missing_number_bitset: [INPUT]\n"
                        "Search the input file [INPUT] of at most %d %d-bit unsigned integers for a missing "
                        "integer, and prints it.\n", MAX_VALUE - 1, N);
        return EXIT_FAILURE;
    }
    // Open the input file
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        fprintf(stderr, "Unable to open the input file %s.\n", argv[1]);
        return EXIT_FAILURE;
    }

    int exit_status = EXIT_SUCCESS;
    BitSet bs;
    bs_init(&bs, MAX_VALUE);
    // Read the input file
    char *line = NULL;
    size_t n = 0;
    ssize_t line_length;
    size_t line_count = 0;
    while ((line_length = getline(&line, &n, input_file)) != -1) {
        if (line_count++ == MAX_VALUE) {
            fprintf(stderr, "Too many input lines\n");
            exit_status = EXIT_FAILURE;
            goto cleanup;
        }
        // Strip new line if it exists
        if (line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
        }
        // Convert the input to an integer
        errno = 0;
        char *end = NULL;
        long long value = strtoll(line, &end, 10);
        if (errno != 0 || line == end) {
            fprintf(stderr, "Invalid input: %s\n", line);
            exit_status = EXIT_FAILURE;
            goto cleanup;
        } else if (value < 0 || value > MAX_VALUE) {
            fprintf(stderr, "Input %s is out of range\n", line);
            exit_status = EXIT_FAILURE;
            goto cleanup;
        }
        // Valid number - add it to the bitset
        bs_set(&bs, value);
    }

    // Print the first missing number
    for (size_t i = 0; i <= MAX_VALUE; i++) {
        if (!bs_is_set(&bs, i)) {
            printf("%zu\n", i);
            break;
        }
    }

cleanup:
    bs_destroy(&bs);
    free(line);
    fclose(input_file);
    return exit_status;
}