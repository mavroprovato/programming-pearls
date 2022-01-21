/**
 * This programs finds a missing number from an input file that contains at most 2^N - 1 N-bit unsigned integers in
 * random order. N is defined in compile time and should be either 8, 16, 32 or 64. The solution uses temporary files to
 * find the missing number, so it should be used when we want to keep the memory usage low.
 *
 * The input file is split into two files: One that contains the numbers with the 1st bit set and one with the ones that
 * have it unset. If one of them is empty, then the missing number has its 1st bit set or unset respectively, so the
 * search stops. If both of them are not empty, we use the smaller one to count the numbers with the 2nd bit set or
 * unset and so on. Eventually we will find an empty file, an the input numbers are less that the search set.
 *
 * This is a solution for problem A.
 */
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define N 32
#define NUMBER uint32_t
#define MAX_VALUE UINT32_MAX

/**
 * Read a number from a null terminated character array.
 *
 * @param line Null terminated string from which the number will be read.
 * @param number Pointer to the memory where the number will be stored. This memory will not be changed if the number is
 * not valid.
 * @return true if the number was read successfully, false otherwise.
 */
bool read_number(char *line, NUMBER *number) {
    errno = 0;
    char *end = NULL;
    long long value = strtoll(line, &end, 10);
    if (errno != 0 || line == end) {
        fprintf(stderr, "Invalid input: %s\n", line);
        return false;
    } else if (value < 0 || value > MAX_VALUE) {
        fprintf(stderr, "Input %s is out of range\n", line);
        return false;
    }

    *number = (NUMBER) value;

    return true;
}

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
        fprintf(stderr, "Usage: missing_number_file: [INPUT]\n"
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

    int exit_status = EXIT_SUCCESS; // The exit status for the program
    char *line = NULL; // Will hold the current line from the file
    size_t n = 0; // Needed for the getline invocation
    size_t current_bit = 0; // The current bit to check. 0 is the least significant bit
    FILE *current_file = input_file; // The current file to check
    NUMBER missing_number = 0; // The value of the missing number

    while (current_bit < N) {
        ssize_t line_length; // The current line length
        size_t line_count = 0; // The total line count
        size_t count_bit_set = 0; // The count of numbers with the current bit set
        size_t count_bit_unset = 0; // The count of numbers with the current bit unset

        // Temporary file in which we will store the numbers with the current bit set
        FILE *bit_set = tmpfile();
        if (!bit_set) {
            fprintf(stderr, "Could not create temporary file\n");
            exit_status = EXIT_FAILURE;
            goto cleanup;
        }
        // Temporary file in which we will store the numbers with the current bit unset
        FILE *bit_unset = tmpfile();
        if (!bit_unset) {
            fprintf(stderr, "Could not create temporary file\n");
            exit_status = EXIT_FAILURE;
            goto cleanup;
        }

        // Read the current input file line by line
        while ((line_length = getline(&line, &n, current_file)) != -1) {
            // Check if we got to many lines (=numbers)
            if (line_count++ == MAX_VALUE) {
                fprintf(stderr, "Too many input lines\n");
                exit_status = EXIT_FAILURE;
                goto cleanup;
            }
            // Strip new line if it exists
            if (line[line_length - 1] == '\n') {
                line[line_length - 1] = '\0';
            }
            // Read the number
            NUMBER number;
            read_number(line, &number);
            // Check if the bit is set or not
            if ((number >> current_bit) & 1) {
                fprintf(bit_set, "%d\n", number);
                count_bit_set++;
            } else {
                fprintf(bit_unset, "%d\n", number);
                count_bit_unset++;
            }
        }

        // Check the counts
        if (count_bit_set == 0) {
            // No numbers with the current bit set, so the missing number must have the current bit set
            missing_number = (1 << current_bit) | missing_number;
            break;
        } else if (count_bit_unset == 0) {
            // No numbers with the current bit unset, so the missing number must have the current bit unset
            break;
        } else if (count_bit_set < count_bit_unset) {
            // Search the missing numbers in the range of numbers that have the current bit set
            missing_number = (1 << current_bit) | missing_number;
            current_file = bit_set;
        } else {
            // Search the missing numbers in the range of numbers that have the current bit unset
            current_file = bit_unset;
        }
        rewind(current_file); // Rewind the current file in order to read it again
        current_bit++; // Check the next bit
    }
    printf("%d\n", missing_number);

cleanup:
    free(line);
    fclose(input_file);
    return exit_status;
}

