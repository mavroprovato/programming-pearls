/**
 * This program reads a list of positive 32-bit integers from an input file, sorts them, and then writes them to the
 * standard output. Each integer must be in its own line. It uses a bit vector in order to sort the input. In order to
 * minimize the memory usage, n number of passes of the input file can be performed. As the program needs to read the
 * input multiple times, the standard input cannot be used. An input file must be provided as a command line argument.
 *
 * This program is a solution for problems 3 and 5.
 */

#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitset.h"

// The maximum number of elements that the program can handle.
#define MAX_ELEMENTS 1000000
// The maximum value that an element can have.
#define MAX_VALUE (10000000 - 1)

/**
 * The main entry point of the program. It takes 2 required command line arguments: The input file and the number of
 * passes that we want to perform.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return The program exit status.
 */
int main(int argc, char *argv[]) {
    if (argc < 3) {
        // We need 2 command line arguments
        fprintf(stderr, "Usage: bitset_sort: [INPUT] [N]\n"
                        "Sort the [INPUT] file that contains a list of positive 32-bit integers and prints a sorted "
                        "output in [N] passes.\n");
        return EXIT_FAILURE;
    }

    // Parse input arguments
    char *end_ptr = NULL;
    errno = 0;
    u_int32_t passes = strtoul(argv[2], &end_ptr, 10);
    if (end_ptr == argv[1] || errno != 0) {
        fprintf(stderr, "Invalid value for number of passes.\n");
        return EXIT_FAILURE;
    }
    if (passes > MAX_ELEMENTS) {
        fprintf(stderr, "Cannot do more passes than the maximum number of elements (%d).\n", MAX_ELEMENTS);
        return EXIT_FAILURE;
    }

    // Open the input file
    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        fprintf(stderr, "Unable to open input file\n");
        return EXIT_FAILURE;
    }

    // Initialize the bitset
    u_int32_t step = ceil((double) (MAX_VALUE + 1) / passes);
    BitSet *bs = malloc(sizeof(BitSet));
    bs_init(bs, step);

    // Perform multiple passes for the input
    int exit_status = EXIT_SUCCESS;
    char *line = NULL;
    for (size_t i = 0; i < passes; i++) {
        // Read the input line by line
        size_t len = 0;
        size_t current_index = 0;
        while (getline(&line, &len, input) != -1) {
            // Parse line as an integer
            errno = 0;
            u_int32_t number = strtoul(line, &end_ptr, 10);
            // Perform error checking
            if (end_ptr == line || errno != 0) {
                fprintf(stderr, "Could not parse %s line %zu as an number\n", line, current_index);
                exit_status = EXIT_FAILURE;
                goto cleanup;
            }
            // Number read successfully, set the bit
            if (i * step <= number <= (i + 1) * step) {
                bs_set(bs, number - step * i);
            }
        }

        // Output the numbers that are contained in the bit set
        for (size_t j = 0; j < bs->n; j++) {
            if (bs_is_set(bs, j)) {
                printf("%zu\n", i * step + j);
            }
        }

        if (i != passes - 1) {
            // Go to the start of the input and reset the bitset for the next pass
            fseek(input, 0, SEEK_SET);
            bs_reset(bs);
        }
    }

    // Cleanup
    cleanup:
    free(line);
    bs_destroy(bs);
    free(bs);
    exit(exit_status);
}
