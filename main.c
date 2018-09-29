#include <stdio.h>
#include "flags.h"

struct program_flags {
  bool is_base64;
  bool is_hex;
  const char *method;
  const char *key;
  const char *iv;
  const char *text;
} g_program_flags;

bool parse_command_line(flags_parser_t *parser, int argc, char **argv) {
  flags_parser_define_bool(parser, "base64", "Base64 format");
  flags_parser_define_bool(parser, "hex", "Hex format");
  flags_parser_define_string(parser, "method", "Method");
  flags_parser_define_string(parser, "key", "KEY");
  flags_parser_define_string(parser, "iv", "IV");
  flags_parser_define_string(parser, "text", "Plain text");

  if (!flags_parser_parse(parser, argc, argv)) {
    return false;
  }

  g_program_flags.is_base64 = flags_parser_get_bool(parser, "base64");
  g_program_flags.is_hex = flags_parser_get_bool(parser, "hex");
  g_program_flags.text = flags_parser_get_string(parser, "text");
  g_program_flags.method = flags_parser_get_string(parser, "method");
  g_program_flags.key = flags_parser_get_string(parser, "key");
  g_program_flags.iv = flags_parser_get_string(parser, "iv");
  printf("g_program_flags: %d %d %s %s %s %s\n", g_program_flags.is_base64, g_program_flags.is_hex, g_program_flags.text, g_program_flags.method, g_program_flags.key, g_program_flags.iv);
  return true;
}

void run() {}

int main(int argc, char **argv) {
  flags_parser_t *parser = flags_parser_create("Crypto Program");
  if (!parser) {
    fprintf(stderr, "flags_parser_create failed\n");
    return -1;
  }
  if (parse_command_line(parser, argc, argv)) {
    run();
  } else {
    flags_parser_helper(parser, stderr);
  }

  flags_parser_destroy(parser);
  return 0;
}
