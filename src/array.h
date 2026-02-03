#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

/**
 * @brief Dynamic array of strings (null-terminated C strings)
 */
typedef struct {
  char **item;
  int capacity;
  int size;
} StringList_t;

/**
 * @brief free array from memory
 */
void free_array(StringList_t *list);

/**
 * @brief added string to array
 * @param list pointer to StringList that needs to be added to
 * @param str pointer to string that wants adding to array
 * @return 0 if successful been added and -1 if its failed
 */
int add_array(StringList_t *list, const char *str);

/**
 * @brief writes array to binary file
 * @param list pointer to a array that needs writing to binary
 * @param filename path to input file
 * @return 0 if successful been added and -1 if its failed
 */
int write_array_binary(const StringList_t *list, const char *filename);

/**
 * @brief Reads a StringList from a binary file previously written by
 * write_binary
 * @param filename Path to the input file
 * @return New StringList (empty on failure)
 * @note On failure, returns empty list (size == 0)
 */
StringList_t read_array_binary(const char *filename);

/**
 * @brief prints arrays
 * @param list pointer to array that wants printing
 */
void print_array(const StringList_t *list);

#endif
