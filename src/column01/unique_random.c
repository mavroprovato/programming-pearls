/**
 * This program is a solution for problem 4. It generates k unique random integers between 0 and n-1. The implementation
 * uses the Fisher–Yates shuffle to preform this task.
 *
 * Note that the standard library function rand() is used, seeded with srand(time(NULL)), so the random numbers
 * generated are predictable. So, this program should not be used if security is a concern.
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * The main entry point of the program. It takes 2 required command line arguments: The number of unique integers to
 * generate and the maximum value of the integer.
 *
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return The program exit status.
 */
int main(int argc, char *argv[]) {
    if (argc < 3) {
        // We need 2 command line arguments
        fprintf(stderr, "Usage: unique_random: [NUMBER] [MAX]\n"
                        "Print [NUMBER] unique random integers between 0 and [MAX] - 1, each in a new line.\n");
        return EXIT_FAILURE;
    }

    // Parse input arguments
    char *end_ptr = NULL;
    errno = 0;
    u_int32_t k = strtoul(argv[1], &end_ptr, 10);
    if (end_ptr == argv[1] || errno != 0) {
        fprintf(stderr, "Invalid value for number of integers to generate.\n");
        return EXIT_FAILURE;
    }
    errno = 0;
    u_int32_t n = strtoul(argv[2], &end_ptr, 10);
    if (end_ptr == argv[2] || errno != 0) {
        fprintf(stderr, "Invalid value for the maximum value of the integers to generate.\n");
        return EXIT_FAILURE;
    }

    // Validate the numbers
    if (k >= n) {
        fprintf(stderr, "Too many integers to generate.\n");
        return EXIT_FAILURE;
    }

    // Create the array
    u_int32_t *array = malloc(n * sizeof(u_int32_t));
    for (size_t i = 0; i < n; i++) {
        array[i] = i;
    }
    // Shuffle the array
    srand(time(NULL));
    for (size_t i = 0; i < k; i++) {
        // Get a random index between i and n - 1
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        // Swap elements i and j, and print i
        u_int32_t temp = array[j];
        array[j] = array[i];
        array[i] = temp;
        printf("%u\n", array[i]);
    }

    return EXIT_SUCCESS;
}
