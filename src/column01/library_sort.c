/**
 * This program reads a list of positive 32-bit integers from standard input, sorts them, and then writes them to the
 * standard output. Each integer must be in its own line. It uses the qsort library function in order to perform the
 * sort, so all integers are loaded in memory.
 *
 * This is a solution for problem 1.
 */

#include "compare.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

// The maximum number of elements that the program can handle.
#define MAX_ELEMENTS 1000000

/**
 * The main entry point of the program.
 */
int main() {
    char *line = NULL;
    size_t len = 0;
    char *end_ptr = NULL;
    size_t current_index = 0;
    int exit_status = EXIT_SUCCESS; // The exit status.
    u_int32_t input[MAX_ELEMENTS] = {0};

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
        // Number read successfully, store it to the input array.
        input[current_index++] = number;
    }

    // Sort the input number array
    qsort(input, current_index, sizeof(u_int32_t), compare_u_int32_t);
    // Print the sorted array
    for (size_t i = 0; i < current_index; i++) {
        printf("%d\n", input[i]);
    }

    // Cleanup
cleanup:
    free(line);
    exit(exit_status);
}
