
#include "strings.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

bool string_has_prefix(const char *s, const char *prefix) {
  size_t prefix_size = strlen(prefix);
  if (prefix_size > strlen(s)) return false;
  return memcmp(s, prefix, strlen(prefix)) == 0;
}

int string_to_integer(const char *s) { return (int)strtol(s, NULL, 0); }

bool string_equal_ignore_case(const char *s1, const char *s2) {
  size_t s1_size = strlen(s1);
  size_t s2_size = strlen(s2);
  if (s1_size != s2_size) return false;
  for (size_t i = 0; i < s1_size; i++) {
    if (tolower(s1[i]) != tolower(s2[i])) {
      return false;
    }
  }
  return true;
}