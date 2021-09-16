cmake_minimum_required (VERSION 3.16)
project (programing_pearls C)

set (CMAKE_C_STANDARD 11)

include_directories (include)

# Create the library of common functions
add_library (pplib src/common/compare.c src/common/bitset.c src/common/stringsig.c)

# Column 1 executables
add_executable (library_sort src/column01/library_sort.c)
target_link_libraries (library_sort LINK_PUBLIC pplib)
add_executable (bitset_sort src/column01/bitset_sort.c)
target_link_libraries (bitset_sort LINK_PUBLIC pplib m)
add_executable (unique_random src/column01/unique_random.c)

# Column 2 executables
add_executable (anagram src/column02/anagram.c)
target_link_libraries (anagram LINK_PUBLIC pplib)
