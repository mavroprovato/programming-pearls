/**
 * This program is a solution for problem 3. It reads a list of positive 32-bit integers from standard input, sorts
 * them, and then writes them to the standard output. Each integer must be in its own line. This version of the program
 * uses a bit vector. When a number is read, the corresponding bit is set. Then the numbers are printed in sorted order.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitset.h"

// The maximum number of elements that the program can handle.
#define MAX_ELEMENTS 1000000
// The maximum value that an element can have.
#define MAX_VALUE 10000000

/**
 * The main entry point of the program.
 */
int main() {
    char *line = NULL;
    size_t len = 0;
    char *end_ptr = NULL;
    size_t current_index = 0;
    int exit_status = EXIT_SUCCESS; // The exit status.
    BitSet bs;
    bs_init(&bs, MAX_VALUE);

    // Read the input line by line
    while (getline(&line, &len, stdin) != -1) {
        errno = 0;
        // Parse line as an integer
        u_int32_t number = strtoul(line, &end_ptr, 10);
        // Perform error checking
        if (end_ptr == line || errno != 0) {
            fprintf(stderr, "Could not parse line %zu as an number\n", current_index);
            exit_status = EXIT_FAILURE;
            goto cleanup;
        }
        // Number read successfully, set the bit
        bs_set(&bs, number);
    }

    // Output the numbers that are contained in the bit set
    for (size_t i = 0; i < bs.n; i++) {
        if (bs_is_set(&bs, i)) {
            printf("%zu\n", i);
        }
    }

    // Cleanup
    cleanup:
    free(line);
    exit(exit_status);
}
