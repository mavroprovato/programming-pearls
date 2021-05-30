cmake_minimum_required (VERSION 3.19)
project (programing_pearls C)

set (CMAKE_C_STANDARD 11)

include_directories (include)

add_library (bitset src/column01/bitset.c)

add_executable (library_sort src/column01/library_sort.c)

add_executable (bitset_sort src/column01/bitset_sort.c)
target_link_libraries (bitset_sort LINK_PUBLIC bitset)
