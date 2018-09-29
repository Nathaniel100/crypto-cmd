#ifndef STRINGS__H
#define STRINGS__H

#include <stdbool.h>

bool string_has_prefix(const char *s, const char *prefix);

int string_to_integer(const char *s);

bool string_equal_ignore_case(const char *s1, const char *s2);

#endif