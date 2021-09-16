#ifndef STRINGSIG_H
#define STRINGSIG_H

#include <stdbool.h>
#include <stddef.h>

/**
 * The string signature structure
 */
typedef struct {
    /** The signature. */
    char *signature;
    /** The signature length. */
    size_t n;
} StringSignature;

/**
* Initialize the string signature.
*
* @param ss Pointer to the string signature structure to initialize.
* @param string Pointer to the string for which to calculate the signature.
* @return Pointer to the signature if it was created correctly, NULL otherwise.
*/
StringSignature *ss_init(StringSignature *ss, const char *string);

/**
 * Free resources associated with the string signature.
 *
 * @param bs Pointer to the string signature data structure to be freed.
 */
void ss_destroy(StringSignature *bs);

/**
 * Check if the string matches with the signature.
 *
 * @param string The string to compare.
 * @param ss The signature.
 * @return True if the signature matches, false otherwise.
 */
bool ss_matches(const char *string, StringSignature *ss);

#endif //STRINGSIG_H
