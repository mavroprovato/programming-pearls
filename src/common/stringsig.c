#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "compare.h"
#include "stringsig.h"

/**
* Initialize the string signature.
*
* @param ss Pointer to the string signature structure to initialize.
* @param string Pointer to the string for which to calculate the signature.
* @return Pointer to the signature if it was created correctly, NULL otherwise.
*/
StringSignature *ss_init(StringSignature *ss, const char *string) {
    ss->signature = strdup(string);
    if (!ss->signature) {
        return NULL;
    }
    qsort(ss->signature, strlen((const char *) ss->signature), sizeof(char), compare_char);
    ss->n = strlen(string);

    return  ss;
}

/**
 * Free resources associated with the string signature.
 *
 * @param ss Pointer to the string signature data structure to be freed.
 */
void ss_destroy(StringSignature *ss) {
    free(ss->signature);
}

/**
 * Check if the string matches with the signature.
 *
 * @param string The string to compare.
 * @param ss The signature.
 * @return True if the signature matches, false otherwise.
 */
bool ss_matches(const char *string, StringSignature *ss) {
    bool matches = false;
    StringSignature target;

    if (ss_init(&target, string)) {
        if (target.n == ss->n) {
            matches = strcmp(target.signature, ss->signature) == 0;
        }
        ss_destroy(&target);
    }

    return matches;
}