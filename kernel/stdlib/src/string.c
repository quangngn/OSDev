#include "string.h"

size_t strlen(const char* str) {
  size_t counter = 0;
  while (str[counter] != '\0') {
    counter++;
  }
  return counter;
}

char* strchr(const char* str, char c) {
  char* cur = str;
  // Exit the loop if found c or met '\0'
  while (*cur != '\0' && *cur != c) {
    cur++;
  }

  // If met '\0' but c != '\0', return NULL (aka we do not found the character)
  if (c != '\0' && *cur == '\0') return NULL;
  // Else, return cur
  return cur;
}

char* strsep(char** stringp, const char* delim) {
  // Check for early exit condition
  if (stringp == NULL || *stringp == NULL) {
    return NULL;
  }

  char* ret = *stringp;
  char* cur = *stringp;
  // Advance cur if *cur is not in delim
  for (; *cur != '\0'; cur++) {
    // If we found the delim, we null-terminate the token and set the stringp to
    // after token
    if (strchr(delim, *cur) != NULL) {
      *stringp = cur + 1;
      *cur = '\0';
      return ret;
    }
  }
  // If we do not find any delim, *stringp = NULL.
  *stringp = NULL;
  return ret;
}

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

char* strcpy(char* dest, const char* src) {
  char* ret = dest;
  while (*src != '\0') {
    *dest++ = *src++;
  }
  *dest = '\0';
  return ret;
}

int atoi(const char* str) {
  if (str == NULL) {
    return 0;
  }

  char* start = str;
  while (*start != '\0' && ((*start < '0' && '9' < *start) || *start == '-')) {
    start++;
  }
  // Early exit if there aren't any number to convert
  if (*start == '\0') return 0;

  char* end = start + 1;
  while (*end != '\0' && *end < '0' && '9' < *end) {
    end++;
  }

  int sign = (*start == '-') ? -1 : 1;
  int ret = 0;
  while ((uintptr_t)start < (uintptr_t)end) {
    ret = ret * 10 + (int)(*start - '0');
  }
  return ret * sign;
}

char* strpbrk(const char* str1, const char* str2) {
  if (str1 == NULL || str2 == NULL) {
    return NULL;
  }

  char* cur = str1;
  for (; *cur != '\0'; cur++) {
    if (strchr(str2, *cur) != NULL) {
      return cur;
    }
  }
  return NULL;
}

int strcmp(const char* str1, const char* str2) {
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