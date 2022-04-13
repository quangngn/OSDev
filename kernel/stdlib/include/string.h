#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Count the number of characters in the input string.
 * \param str Input string.
 * \returns number of characters in str (excluding null character).
 */
size_t strlen(const char *str);

/**
 * Functions find the pointer the the first occurence of c in str.
 * \param str Given string.
 * \param c Character to be looked for in str.
 * \returns the pointer to the first occurence of c in str. Return NULL if the
 * character c is not found.
 */
char *strchr(const char *str, char c);

/**
 * The function finds the first character in delim, replace it with '\0'. The
 * pstr would be set to the next character after the replaced delim. If
 * *pstr or pstr is null, we return null. If there is no delimiter, the
 * token is set to the entire *pstr, and *pstr is set to null. Delimiter
 * is replaced once at a time.
 *
 * \param pointer to pointer of the given string.
 * \param delim string that contains delim characters.
 * \returns first token seperated by delim. Return NULL if pstr or *pstr is
 * NULL.
 */
char *strsep(char **pstr, const char *delim);

/**
 * Function returns the pointer to the first token in the string. A null pointer
 * is return if str is NULL. If in the subsequent call, we call on the same
 * string, str must be set to NULL. If multiple characters in delim exist next
 * to each other in str, we must remove them all.
 *
 * \param pointer to pointer of the given string.
 * \param delim string that contains delim characters.
 * \returns first token seperated by delim. Return NULL if str & strtok_remain
 * is NULL.
 */
char *strtok(char *str, const char *delim);

/**
 * Copy the string from src to dest.
 * \param dst Address of the destination string.
 * \param src Address of the source string.
 * \returns Address of the destination string.
 */
char *strcpy(char *dest, const char *src);

/**
 * Convert the first occurence of numeric character into signed integer. If
 * there is no numeric characters, return 0. For example:
 *    atoi("0xFF") returns 0
 *    atoi("12b9") returns 12
 *    atoi("-10b") returns -10
 *    atoi("--10") returns 0
 *    atoi("abcc") returns 0
 * \param str Given string.
 * \returns signed integer from the given string.
 */
int atoi(const char *str);

/**
 * Function returns the pointer to the first occurence of the character in str1
 * that also exists in str2.
 */
char *strpbrk(const char *str1, const char *str2);

/**
 * Function to call string comparison. In case 1 string is the prefix of the
 * other string, the prefix is consider less than the other string.
 */
int strcmp(const char *str1, const char *str2);