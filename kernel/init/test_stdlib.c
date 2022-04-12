#include "test_stdlib.h"

// Test stdio.h
void test_stdio() {
  printf("[stdio.h test]:\n");
  printf("Print nothing:\n");
  printf("");
  printf("Print formatted string:\n");
  printf("String: %s, Dec: %d, Hex: %x, Pointer: %p, %%\n", "H3||0",
         23499823984, 0x1232908AB, &test_stdio);

  char* s = NULL;
  size_t size = 0;
  int stream = STD_IN;
  getline(&s, &size, &stream);
  printf(s);

  printf("\n");
}

// Test
void test_string() {
  printf("[string.h test]:\n");
  char s0[] = "Hello my name is Quang!";
  char s1[] = "Quang#1234()";
  char s2[] = "Quang#1234()abc";
  char s3[] = "Quang#12";
  char s4[] = "ABCDE";
  char s5[] = "FGHIJ";

  // Test strlen()
  int pass_count = 0;
  pass_count += assert(strlen("") == 0, "strlen test 1 failed.\n");
  pass_count += assert(strlen("\n") == 1, "strlen test 2 failed.\n");
  pass_count += assert(strlen(s0) == 23, "strlen test 3 failed.\n");
  if (pass_count == 3) {
    printf("strlen test passed!\n");
  } else {
    perror("sring.h test failed!\n");
    return;
  }

  // Test strchr()
  pass_count += assert(strchr("", 'a') == NULL, "strchr test 1 failed.\n");
  pass_count += assert(strchr(s0, 'e') == &s0[1], "strchr test 2 failed.\n");
  pass_count += assert(strchr(s0, 'a') == &s0[10], "strchr test 3 failed.\n");
  if (pass_count == 6) {
    printf("strchr test passed!\n");
  } else {
    perror("sring.h test failed!\n");
    return;
  }

  // test strcmp()
  pass_count += assert(strcmp("", "") == 0, "strcmp test 1 failed.\n");
  pass_count += assert(strcmp(s1, s1) == 0, "strcmp test 2 failed.\n");
  pass_count += assert(strcmp(s1, s2) == -1, "strcmp test 3 failed.\n");
  pass_count += assert(strcmp(s1, s3) == 1, "strcmp test 4 failed.\n");
  pass_count += assert(strcmp(s4, s5) == -1, "strcmp test 5 failed.\n");
  pass_count += assert(strcmp(s5, s4) == 1, "strcmp test 6 failed.\n");
  if (pass_count == 12) {
    printf("strcmp test passed!\n");
  } else {
    perror("sring.h test failed!\n");
    return;
  }

  // test strcpy()
  char temp_buffer0[100];
  char temp_buffer1[100];
  strcpy(temp_buffer0, s0);
  strcpy(temp_buffer1, s1);
  pass_count +=
      assert(strcmp(temp_buffer0, s0) == 0, "strcpy test 1 failed.\n");
  pass_count +=
      assert(strcmp(temp_buffer1, s1) == 0, "strcpy test 2 failed.\n");
  pass_count +=
      assert(strcmp(temp_buffer1, s2) == -1, "strcpy test 3 failed.\n");
  pass_count +=
      assert(strcmp(temp_buffer1, s3) == 1, "strcpy test 4 failed.\n");
  if (pass_count == 16) {
    printf("strcpy test passed!\n");
  } else {
    perror("sring.h test failed!\n");
    return;
  }

  // test strsep()
  char* cursor = s0;
  char* temp = strsep(&cursor, "-; ");
  pass_count += assert(cursor == &s0[6], "strsep test 1.1 failed.\n");
  pass_count += assert(temp == &s0[0], "strsep test 1.2 failed.\n");
  pass_count += assert(strcmp(temp, "Hello") == 0, "strsep test 1.3 failed.\n");

  temp = strsep(&cursor, "-; ");
  pass_count += assert(cursor == &s0[9], "strsep test 2.1 failed.\n");
  pass_count += assert(temp == &s0[6], "strsep test 2.2 failed.\n");
  pass_count += assert(strcmp(temp, "my") == 0, "strsep test 2.3 failed.\n");

  temp = strsep(&cursor, "-; ");
  pass_count += assert(cursor == &s0[14], "strsep test 3.1 failed.\n");
  pass_count += assert(temp == &s0[9], "strsep test 3.2 failed.\n");
  pass_count += assert(strcmp(temp, "name") == 0, "strsep test 3.3 failed.\n");

  temp = strsep(&cursor, "-; ");
  pass_count += assert(cursor == &s0[17], "strsep test 4.1 failed.\n");
  pass_count += assert(temp == &s0[14], "strsep test 4.2 failed.\n");
  pass_count += assert(strcmp(temp, "is") == 0, "strsep test 4.3 failed.\n");

  temp = strsep(&cursor, "-; ");
  pass_count += assert(cursor == NULL, "strsep test 5.1 failed.\n");
  pass_count += assert(temp == &s0[17], "strsep test 5.2 failed.\n");
  pass_count +=
      assert(strcmp(temp, "Quang!") == 0, "strsep test 5.3 failed.\n");
  if (pass_count == 31) {
    printf("strsep test passed!\n");
  } else {
    perror("sring.h test failed!\n");
    return;
  }

  // Test strtok
  char s6[] = "Hello my name is Quang!";
  cursor = s6;
  temp = strtok(cursor, "-; ");
  pass_count += assert(cursor == &s6[0], "strtok test 1.1 failed.\n");
  pass_count += assert(temp == &s6[0], "strtok test 1.2 failed.\n");
  pass_count += assert(strcmp(temp, "Hello") == 0, "strtok test 1.3 failed.\n");

  temp = strtok(NULL, "-; ");
  pass_count += assert(cursor == &s6[0], "strtok test 2.1 failed.\n");
  pass_count += assert(temp == &s6[6], "strtok test 2.2 failed.\n");
  pass_count += assert(strcmp(temp, "my") == 0, "strtok test 2.3 failed.\n");

  temp = strtok(NULL, "-; ");
  pass_count += assert(cursor == &s6[0], "strtok test 3.1 failed.\n");
  pass_count += assert(temp == &s6[9], "strtok test 3.2 failed.\n");
  pass_count += assert(strcmp(temp, "name") == 0, "strtok test 3.3 failed.\n");

  temp = strtok(NULL, "-; ");
  pass_count += assert(cursor == &s6[0], "strtok test 4.1 failed.\n");
  pass_count += assert(temp == &s6[14], "strtok test 4.2 failed.\n");
  pass_count += assert(strcmp(temp, "is") == 0, "strtok test 4.3 failed.\n");

  temp = strtok(NULL, "-; ");
  pass_count += assert(cursor == &s6[0], "strtok test 5.1 failed.\n");
  pass_count += assert(temp == &s6[17], "strtok test 5.2 failed.\n");
  pass_count +=
      assert(strcmp(temp, "Quang!") == 0, "strtok test 5.3 failed.\n");

  temp = strtok(NULL, "-; ");
  pass_count += assert(temp == NULL, "strtok test 6 failed.\n");
  if (pass_count == 47) {
    printf("strtok test passed!\n");
  } else {
    perror("sring.h test failed!\n");
    return;
  }

  // Test atoi()
  pass_count += assert(atoi("0xFF") == 0, "atoi test 1 failed.\n");
  pass_count += assert(atoi("12b9") == 12, "atoi test 2 failed.\n");
  pass_count += assert(atoi("-1123b") == -1123, "atoi test 3 failed.\n");
  pass_count += assert(atoi("--10") == 0, "atoi test 4 failed.\n");
  pass_count += assert(atoi("abcc") == 0, "atoi test 5 failed.\n");
  if (pass_count == 52) {
    printf("atoi test passed!\n");
  } else {
    perror("sring.h test failed!\n");
    return;
  }
}