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

#include <getopt.h>

#include "bitset.h"

#define MAX_ELEMENTS 1000000
#define MAX_VALUE 10000000
// The maximum number of elements that the program can handle.
static uint32_t max_elements = MAX_ELEMENTS;
// The maximum value that an element can have.
static uint32_t max_value = MAX_VALUE;
// The number of passes to perform.
static size_t passes = 1;
// The help flag
static bool help_flag = false;
// The file to open
static char *input = NULL;

/**
 * Parse the command line arguments.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return true if the parsing was successful, false otherwise.
 */
bool parse_arguments(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"count", optional_argument, 0, 'c'},
        {"max-value", optional_argument, 0, 'm'},
        {"passes", optional_argument, 0, 'p'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    // Parse options
    int c;
    char *end_ptr = NULL;
    int option_index = 0;
    while (true) {
        c = getopt_long(argc, argv, "hc:m:p:", long_options, &option_index);

        if (c == -1) {
            break;
        }
        switch (c) {
            case 'c':
                max_elements = strtoul(optarg, &end_ptr, 10);
                if (end_ptr == optarg || errno != 0) {
                    fprintf(stderr, "Invalid value for the max elements argument: %s.\n", optarg);
                    return false;
                }
                break;
            case 'm':
                max_value = strtoul(optarg, &end_ptr, 10);
                if (end_ptr == optarg || errno != 0) {
                    fprintf(stderr, "Invalid value for the max value argument: %s.\n", optarg);
                    return false;
                }
                break;
            case 'p':
                passes = strtoul(optarg, &end_ptr, 10);
                if (end_ptr == optarg || errno != 0) {
                    fprintf(stderr, "Invalid value for the passes argument: %s.\n", optarg);
                    return false;
                }
                break;
            case 'h':
                help_flag = true;
                return false;
            default:
                return false;
        }
    }

    // Parse the remaining arguments
    if (optind < argc) {
        input = argv[optind];
    }
    // Validate the arguments
    if (max_elements > max_value) {
        fprintf(stderr, "The maximum number of elements must be less than the maximum value.\n");
        return false;
    }
    if (passes > 1 && !input) {
        fprintf(stderr, "When performing multiple passes an input file must be provided.\n");
        return false;
    }

    return true;
}

/**
 * Prints usage instructions for the program.
 */
void print_usage() {
    printf("Usage: [OPTION]... [FILE]...\n\n"
           "Read a list of positive 32-bit integers from an input file and sorts them.\n\n"
           "Mandatory arguments to long options are mandatory for short options too.\n"
           "    -c, --count=COUNT       The number of elements to process, default is %u.\n"
           "    -m, --max-value=VALUE   The maximum value of the elements, default is %u.\n"
           "    -p, --passes=PASSES     The number of passes to perform for the input, default is 1.\n"
           "                                If the number of passes is more than one, an input file must be provided.\n"
           "    -h, --help              Display this help and exit.\n"
           "", MAX_ELEMENTS, MAX_VALUE);
}

/**
 * The main entry point of the program. It takes 2 required command line arguments: The input file and the number of
 * passes that we want to perform.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return The program exit status.
 */
int main(int argc, char *argv[]) {
    // Parse the command line arguments
    if (!parse_arguments(argc, argv)) {
        if (help_flag) {
            print_usage();
            return EXIT_SUCCESS;
        } else {
            return EXIT_FAILURE;
        }
    }
    // Open the input file
    FILE *file = input ? fopen(input, "r") : stdin;
    if (file == NULL) {
        fprintf(stderr, "Unable to open input file %s\n", input);
        return EXIT_FAILURE;
    }

    // Initialize the bitset
    u_int32_t step = ceil((double) (max_value + 1) / (double) passes);
    BitSet *bs = malloc(sizeof(BitSet));
    bs_init(bs, step);

    // Perform multiple passes for the input
    int exit_status = EXIT_SUCCESS;
    char *line = NULL;
    char *end_ptr = NULL;
    for (size_t i = 0; i < passes; i++) {
        // Read the input line by line
        size_t len = 0;
        while (getline(&line, &len, file) != -1) {
            // Parse line as an integer
            errno = 0;
            u_int32_t number = strtoul(line, &end_ptr, 10);
            // Perform error checking
            if (end_ptr == line || errno != 0) {
                fprintf(stderr, "Could not parse line as an number\n");
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
            fseek(file, 0, SEEK_SET);
            bs_reset(bs);
        }
    }

    // Cleanup
    cleanup:
    free(line);
    bs_destroy(bs);
    free(bs);
    fclose(file);
    exit(exit_status);
}
