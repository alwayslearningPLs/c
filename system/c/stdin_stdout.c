#include <stdio.h>
#include <stdlib.h>

// putchar(c); puts(const char* s); fputc(); putc(); fputs();
void print_stuff(char *input) {
  // using stdout by default
  char *tmp = input; while (*tmp != '\0') putchar(*tmp++);
  puts(input);

  // using stdin explicitly
  tmp = input; while (*tmp != '\0') fputc(*tmp++, stdout);
  tmp = input; while (*tmp != '\0') putc(*tmp++, stdout);
  fputs(input, stdout);
}

// getchar(); fgetc(stdin); getc(stdin); fgets(char *restrict s, int size, FILE *stream);
void read_stuff() {
  fputs("getchar(): ", stdout);
  getchar();

  fputs("fgetc(stdin): ", stdout);
  fgetc(stdin);

  fputs("getc(stdin): ", stdout);
  getc(stdin);

  // char *s = (char *) malloc(10 * sizeof(char));
  fputs("deprecated gets(buf): ", stdout);
  // gets(s);

  char *s = (char *) malloc(10 * sizeof(char));
  fputs("fgets(buf, BUFSIZ, stdin): ", stdout);
  fgets(s, 10, stdin);
}

int main() {
  print_stuff("Hello world\n");
  read_stuff();
  return 0;
}
