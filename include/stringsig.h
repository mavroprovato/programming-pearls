#ifndef STRINGSIG_H
#define STRINGSIG_H

#include <stddef.h>

/**
 * Calculate the signature for a string.
 *
 * @param string The string to calculate the signature for.
 * @param length The length of the input string.
 * @param signature Pointer to where the signature will be written to. The caller is responsible to allocate length + 1
 * characters for the string.
 */
void ss_calculate(const char *string, size_t length, char *signature);

#endif // STRINGSIG_H
