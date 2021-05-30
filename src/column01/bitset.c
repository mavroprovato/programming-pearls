/**
 * This library is a solution for problem 2. It implements the bit set data structure, which is used to compactly store
 * bits. It provides functions to set, unset and toggle bits in the data structure.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "bitset.h"

#define BS_UNIT_POS(n) ((n) / (sizeof(BS_UNIT) * CHAR_BIT))
#define BS_BIT_POS(n) ((n) % (sizeof(BS_UNIT) * CHAR_BIT))

/**
* Initialize the bit set.
*
* @param bs Pointer to the bit set data structure.
* @param n The number of bits that the set holds.
* @return true if the data structure was initialized successfully, false otherwise.
*/
bool bs_init(BitSet *bs, size_t n) {
    // Make sure that the requested size is greater than zero
    if (n == 0) {
        return false;
    }

    // Initialize the storage
    size_t num_bytes = ((n - 1) / (sizeof(BS_UNIT) * CHAR_BIT) + 1) * sizeof(BS_UNIT);
    bs->bits = malloc(num_bytes);
    if (!bs->bits) {
        return false;
    }
    memset(bs->bits, 0, num_bytes);
    bs->n = n;

    return true;
}

/**
 * Free resources associated with the bit set.
 *
 * @param bs Pointer to the bit set data structure to be freed.
 */
void bs_destroy(BitSet *bs) {
    free(bs->bits);
}

/**
* Set a bit at the specified position.
*
* @param bs Pointer to the bit set data structure.
* @param n The position to set.
* @return true if the bit was set successfully, false otherwise.
*/
bool bs_set(BitSet *bs, size_t n) {
    if (n > bs->n) {
        return false;
    }

    bs->bits[BS_UNIT_POS(n)] |= 1ull << BS_BIT_POS(n);

    return true;
}

/**
* Clear a bit at the specified position.
*
* @param bs Pointer to the bit set data structure.
* @param n The position to clear.
* @return true if the bit was cleared successfully, false otherwise.
*/
bool bs_clear(BitSet *bs, size_t n) {
    if (n > bs->n) {
        return false;
    }

    bs->bits[BS_UNIT_POS(n)] &= ~(1UL << BS_BIT_POS(n));

    return true;
}

/**
* Toggle a bit at the specified position.
*
* @param bs Pointer to the bit set data structure.
* @param n The position to toggle.
* @return true if the bit was toggled successfully, false otherwise.
*/
bool bs_toggle(BitSet *bs, size_t n) {
    if (n > bs->n) {
        return false;
    }

    bs->bits[BS_UNIT_POS(n)] ^= 1UL << BS_BIT_POS(n);

    return true;
}

/**
 * Check if the bit is set in the specified position.
 *
 * @param bs Pointer to the bit set data structure.
 * @param n The position to check.
 * @return true if the bit is set, false otherwise.
 */
bool bs_is_set(BitSet *bs, size_t n) {
    if (n > bs->n) {
        return false;
    }

    return bs->bits[BS_UNIT_POS(n)] & (1ull << BS_BIT_POS(n));
}
