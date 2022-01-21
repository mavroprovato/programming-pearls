cmake_minimum_required (VERSION 3.16)
project (programing_pearls C)

set (CMAKE_C_STANDARD 11)

include_directories (include)

# Create the library of common functions
add_library (pplib src/common/compare.c src/common/bitset.c src/column02/stringsig.c)

# Column 1 executables
add_executable (library_sort src/column01/library_sort.c)
target_link_libraries (library_sort LINK_PUBLIC pplib)
add_executable (bitset_sort src/column01/bitset_sort.c)
target_link_libraries (bitset_sort LINK_PUBLIC pplib m)
add_executable (unique_random src/column01/unique_random.c)

# Column 2 executables
add_executable (missing_number_bitset src/column02/missing_number_bitset.c)
target_link_libraries (missing_number_bitset LINK_PUBLIC pplib)
add_executable (missing_number_file src/column02/missing_number_file.c)
add_executable (anagram src/column02/anagram.c)
target_link_libraries (anagram LINK_PUBLIC pplib)
add_executable (build_anagram_db src/column02/build_anagram_db.c)
target_link_libraries (build_anagram_db LINK_PUBLIC pplib)
add_executable (search_anagram_db src/column02/search_anagram_db.c)
target_link_libraries (search_anagram_db LINK_PUBLIC pplib)
