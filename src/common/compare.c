#include "compare.h"

#include <stdlib.h>

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
 * Comparison function for sorting an array of u_int32_t elements.
 *
 * @param p Pointer to the first array element to compare.
 * @param q Pointer to the second array element to compare.
 * @return -1 if the first element is less that the second, 1 if the first element is greater then the second or 0 if
 * the two elements are equal.
 */
int compare_u_int32_t(const void *p, const void *q) {
    u_int32_t x = *(const u_int32_t*) p;
    u_int32_t y = *(const u_int32_t*) q;

    if (x < y) {
        return -1;
    } else if (x > y) {
        return 1;
    } else {
        return 0;
    }
}
