#include "flags.h"
#include "strings.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FLAGS_ITEM_MAX 255

enum flags_value_type {
  FLAGS_VALUE_BOOL,
  FLAGS_VALUE_INT,
  FLAGS_VALUE_STRING,
};

typedef struct flags_item {
  const char *name;
  const char *description;
  union {
    bool bool_value;
    int num_value;
    const char *str_value;
  } value;
  enum flags_value_type value_type;
} flags_item_t;

// TODO hashtable, sorted array or balance tree
struct flags_parser {
  flags_item_t items[255];
  size_t size;
  const char *description;
};

flags_parser_t *flags_parser_create(const char *description) {
  flags_parser_t *parser = (flags_parser_t *)malloc(sizeof(flags_parser_t));
  memset(parser, 0, sizeof(flags_parser_t));
  parser->description = description;
  return parser;
}

void flags_parser_destroy(flags_parser_t *parser) { free(parser); }

static bool flags_parser_insert_item(flags_parser_t *parser,
                                     const flags_item_t *item) {
  assert(parser->size < FLAGS_ITEM_MAX);
  parser->items[parser->size++] = *item;
  return true;
}

static flags_item_t *flags_parser_find_item(flags_parser_t *parser,
                                            const char *name) {
  for (size_t i = 0; i < parser->size; i++) {
    flags_item_t *item = &parser->items[i];
    if (string_equal_ignore_case(name, item->name)) {
      return item;
    }
  }
  return NULL;
}

bool flags_parser_define_bool(flags_parser_t *parser, const char *name,
                              const char *description) {
  flags_item_t item = {name, description, 0, FLAGS_VALUE_BOOL};
  return flags_parser_insert_item(parser, &item);
}

bool flags_parser_define_int(flags_parser_t *parser, const char *name,
                             const char *description) {
  flags_item_t item = {name, description, 0, FLAGS_VALUE_INT};
  return flags_parser_insert_item(parser, &item);
}

bool flags_parser_define_string(flags_parser_t *parser, const char *name,
                                const char *description) {
  flags_item_t item = {name, description, 0, FLAGS_VALUE_STRING};
  return flags_parser_insert_item(parser, &item);
}

bool flags_parser_parse(flags_parser_t *parser, int argc, char **argv) {
  if (argc < 2) return false;
  int i = 1;
  while (i < argc) {
    const char *arg = argv[i];
    if (!string_has_prefix(arg, "--")) {
      i++;
      continue;
    }
    const char *name = arg + 2;
    flags_item_t *item = flags_parser_find_item(parser, name);
    if (!item) {
      i++;
      continue;
    }
    switch (item->value_type) {
      case FLAGS_VALUE_BOOL:
        item->value.bool_value = true;
        i++;
        break;
      case FLAGS_VALUE_INT:
        if (i + 1 >= argc) {
          return false;
        }
        item->value.num_value = string_to_integer(argv[i + 1]);
        i += 2;
        break;
      case FLAGS_VALUE_STRING:
        if (i + 1 >= argc) {
          return false;
        }
        item->value.str_value = argv[i + 1];
        i += 2;
        break;
      default:
        return false;
    }  // switch ends
  }    // while ends
  return true;
}

bool flags_parser_get_bool(flags_parser_t *parser, const char *name) {
  const flags_item_t *item = flags_parser_find_item(parser, name);
  if (!item) return false;
  return item->value.bool_value;
}

int flags_parser_get_int(flags_parser_t *parser, const char *name) {
  const flags_item_t *item = flags_parser_find_item(parser, name);
  if (!item) return 0;
  return item->value.num_value;
}

const char *flags_parser_get_string(flags_parser_t *parser, const char *name) {
  const flags_item_t *item = flags_parser_find_item(parser, name);
  if (!item) return NULL;
  return item->value.str_value;
}

void flags_parser_helper(flags_parser_t *parser, FILE *stream) {
  fprintf(stream, "%s\n\n", parser->description);
  fprintf(stream, "usage: \n");
  for (size_t i = 0; i < parser->size; i++) {
    const flags_item_t *item = &parser->items[i];
    fprintf(stream, "--%-16s\t%-48s\n", item->name, item->description);
  }
}