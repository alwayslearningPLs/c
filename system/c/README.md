# Learning C system programming

`stdio` or Standard I/O library functions:

- _Buffering_ is performed automatically. The size of the buffer is generally specified by the constant `BUFSIZ`, defined in the include file `stdio.h`.
- Input and output conversions are performed.
- Input and output are automatically formatted.

## File pointers

In the Standard I/O Library, a file is called a _stream_, and is described by a pointer to an object of type `FILE`, called a _file pointer_.

Most of the stdio routines require that a file pointer referring to an open stream be passed to them. However, when reading from the stdin or writing to the stdout, stdio provides _shorthands_ routines that assume one of these streams rather than requiring them to be specified.

| Shorthand | Equivalent |
| --------- | ---------- |
| `getchar()` | `fgetc(stdin)`, `getc(stdin)` |
| `gets(buf)` | `fgets(buf, BUFSIZ, stdin)` |
| `printf(args)` | `fprintf(stdout, args)` |
| `putchar(c)` | `fputc(c, stdout)`, `putc(c, stdout)` |
| `puts(buf)` | `fputs(buf, stdout)` |
| `scanf(args)` | `fscanf(stdin, args)` |

