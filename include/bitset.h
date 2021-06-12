#ifndef BITSET_H
#define BITSET_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// The base unit that holds the bitset data
typedef uint32_t BS_UNIT;

/**
 * The bit set structure
 */
typedef struct {
    /** The storage. */
    BS_UNIT *bits;
    /** The number of bits that the set holds. */
    size_t n;
} BitSet;

/**
* Initialize the bit set.
*
* @param bs Pointer to the bit set data structure.
* @param n The number of bits that the set holds.
* @return true if the data structure was initialized successfully, false otherwise.
*/
bool bs_init(BitSet *bs, size_t n);

/**
 * Free resources associated with the bit set.
 *
 * @param bs Pointer to the bit set data structure to be freed.
 */
void bs_destroy(BitSet *bs);

/**
 * Check if the bit is set in the specified position.
 *
 * @param bs Pointer to the bit set data structure.
 * @param n The position to check.
 * @return true if the bit is set, false otherwise.
 */
bool bs_is_set(BitSet *bs, size_t n);

/**
* Set a bit at the specified position.
*
* @param bs Pointer to the bit set data structure.
* @param n The position to set.
* @return true if the bit was set successfully, false otherwise.
*/
bool bs_set(BitSet *bs, size_t n);

/**
* Clear a bit at the specified position.
*
* @param bs Pointer to the bit set data structure.
* @param n The position to clear.
* @return true if the bit was cleared successfully, false otherwise.
*/
bool bs_clear(BitSet *bs, size_t n);

/**
* Toggle a bit at the specified position.
*
* @param bs Pointer to the bit set data structure.
* @param n The position to toggle.
* @return true if the bit was toggled successfully, false otherwise.
*/
bool bs_toggle(BitSet *bs, size_t n);

/**
* Unset all the bits in the set.
*
* @param bs Pointer to the bit set data structure.
* @return true if the bit set was reset successfully, false otherwise.
*/
bool bs_reset(BitSet *bs);

#endif //BITSET_H
