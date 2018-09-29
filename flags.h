#ifndef FLAGS__H
#define FLAGS__H

#include "strings.h"

#include <stdio.h>
#include <stdbool.h>

typedef struct flags_parser flags_parser_t;

flags_parser_t *flags_parser_create(const char *description);
void flags_parser_destroy(flags_parser_t *parser);
bool flags_parser_define_bool(flags_parser_t *parser, const char *name, const char *description);
bool flags_parser_define_int(flags_parser_t *parser, const char *name, const char *description);
bool flags_parser_define_string(flags_parser_t *parser, const char *name, const char *description);
bool flags_parser_parse(flags_parser_t *parser, int argc, char **argv);
bool flags_parser_get_bool(flags_parser_t *parser, const char *name);
int flags_parser_get_int(flags_parser_t *parser, const char *name);
const char *flags_parser_get_string(flags_parser_t *parser, const char *name);
void flags_parser_helper(flags_parser_t *parser, FILE *stream);

#endif