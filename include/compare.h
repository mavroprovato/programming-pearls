#ifndef COMPARE_H
#define COMPARE_H

/**
 * Comparison function for sorting an array of char elements.
 *
 * @param p Pointer to the first array element to compare.
 * @param q Pointer to the second array element to compare.
 * @return -1 if the first element is less that the second, 1 if the first element is greater then the second or 0 if
 * the two elements are equal.
 */
int compare_char(const void *p, const void *q);

/**
 * Comparison function for sorting an array of u_int32_t elements.
 *
 * @param p Pointer to the first array element to compare.
 * @param q Pointer to the second array element to compare.
 * @return -1 if the first element is less that the second, 1 if the first element is greater then the second or 0 if
 * the two elements are equal.
 */
int compare_u_int32_t(const void *p, const void *q);

#endif //COMPARE_H
