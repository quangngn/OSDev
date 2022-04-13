#include "string.h"

#include "stdio.h"

/**
 * Count the number of characters in the input string.
 * \param str Input string.
 * \returns number of characters in str (excluding null character).
 */
size_t strlen(const char* str) {
  if (str == NULL) return 0;

  size_t counter = 0;
  while (str[counter] != '\0') {
    counter++;
  }
  return counter;
}

/**
 * Functions find the pointer the the first occurence of c in str.
 * \param str Given string.
 * \param c Character to be looked for in str.
 * \returns the pointer to the first occurence of c in str. Return NULL if the
 * character c is not found.
 */
char* strchr(const char* str, char c) {
  if (str == NULL) return NULL;

  char* cur = (char*)str;
  // Exit the loop if found c or met '\0'
  while (*cur != '\0' && *cur != c) {
    cur++;
  }

  // If met '\0' but c != '\0', return NULL (aka we do not found the character)
  if (c != '\0' && *cur == '\0') return NULL;
  // Else, return cur
  return cur;
}

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
char* strsep(char** pstr, const char* delim) {
  // Check for early exit condition
  if (pstr == NULL || *pstr == NULL) {
    return NULL;
  }

  char* ret = *pstr;
  char* cur = *pstr;
  // Advance cur if *cur is not in delim
  for (; *cur != '\0'; cur++) {
    // If we found the delim, we null-terminate the token and set the pstr to
    // after token
    if (strchr(delim, *cur) != NULL) {
      *pstr = cur + 1;
      *cur = '\0';
      return ret;
    }
  }
  // If we do not find any delim, *stringp = NULL.
  *pstr = NULL;
  return ret;
}

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
char* strtok(char* str, const char* delim) {
  static char* strtok_remain = NULL;
  if (str != NULL) {
    strtok_remain = str;
  }

  if (strtok_remain == NULL) {
    return NULL;
  }

  // Remove delim at the begining of strtok_remain
  while (*strtok_remain != '\0' && strchr(delim, *strtok_remain) != NULL) {
    strtok_remain++;
  }

  char* ret = strtok_remain;
  char* cur = ret;
  for (; *cur != '\0'; cur++) {
    // If we found the delimiter character, strtok_remain to next character
    // after delim and null-terminate the current token.
    if (strchr(delim, *cur) != NULL) {
      strtok_remain = cur + 1;
      *cur = '\0';
      return ret;
    }
  }
  // If cur reach the end of strtok_remain without seeing any delim,
  // strtok_remain = NULL.
  strtok_remain = NULL;
  return ret;
}

/**
 * Copy the string from src to dest.
 * \param dst Address of the destination string.
 * \param src Address of the source string.
 * \returns Address of the destination string.
 */
char* strcpy(char* dst, const char* src) {
  if (dst == NULL || src == NULL) return NULL;

  char* ret = dst;
  while (*src != '\0') {
    *dst++ = *src++;
  }
  *dst = '\0';
  return ret;
}

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
int atoi(const char* str) {
  if (str == NULL) {
    return 0;
  }

  // Find the first character that is numeric or '-'
  char* start = (char*)str;
  while (*start != '\0' && (*start < '0' || '9' < *start) && *start != '-') {
    start++;
  }
  // Early exit if there aren't any number to convert
  if (*start == '\0') return 0;

  // Advance end pointer to the end of the supposed number
  char* end = start + 1;
  while (*end != '\0' && '0' <= *end && *end <= '9') {
    end++;
  }

  // Check the sign of the number
  int sign;
  if (*start == '-') {
    sign = -1;
    start++;
  } else {
    sign = 1;
  }

  int ret = 0;
  while ((uintptr_t)start < (uintptr_t)end) {
    ret = ret * 10 + (int)(*start - '0');
    start++;
  }
  return ret * sign;
}

/**
 * Function returns the pointer to the first occurence of the character in str1
 * that also exists in str2.
 */
char* strpbrk(const char* str1, const char* str2) {
  if (str1 == NULL || str2 == NULL) {
    return NULL;
  }

  char* cur = (char*)str1;
  for (; *cur != '\0'; cur++) {
    if (strchr(str2, *cur) != NULL) {
      return cur;
    }
  }
  return NULL;
}

/**
 * Function to call string comparison. In case 1 string is the prefix of the
 * other string, the prefix is consider less than the other string.
 */
int strcmp(const char* str1, const char* str2) {
  if (str1 == NULL && str2 != NULL) return -1;
  if (str1 != NULL && str2 == NULL) return 1;
  if (str1 == NULL && str2 == NULL) return 0;
  int idx = 0;
  while (true) {
    if (str1[idx] < str2[idx]) {
      return -1;
    } else if (str1[idx] > str2[idx]) {
      return 1;
    } else if (str1[idx] == '\0' && str2[idx] == '\0') {
      // We reach the end of both strings and all characters are checked to be
      // the same
      return 0;
    }
    idx++;
  }
}