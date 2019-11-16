#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define BUFFER_SIZE 256
#define TAPE_SIZE 30000

#define close_io() do {     \
    if (argc > 1 && in)     \
        fclose(in);         \
    if (argc > 2 && out)    \
        fclose(out);        \
} while(0)

#define write_out(format, ...) do {                                 \
    if (fprintf(out, format __VA_OPT__(,) __VA_ARGS__) < 0) {       \
        if (argc > 2)                                               \
            fprintf(stderr, "Could not write to %s\n", argv[2]);    \
        else                                                        \
            fprintf(stderr, "Could not write to output\n");         \
        close_io();                                                 \
        return 1;                                                   \
    }                                                               \
} while(0)                                                          \

#define BFHEADER "#include <stdio.h>\nint t[%lu], *p=t;\nint main() {\n"
#define BFADD "++(*p);\n"
#define BFSUB "--(*p);\n"
#define BFLFT "--p;\n"
#define BFRGT "++p;\n"
#define BFINP "*p=getchar();\n"
#define BFOUT "putchar(*p);\n"
#define BFSLP "while(*p){\n"
#define BFELP "}\n"
#define BFFOOTER "return 0;\n}"

int
main(int argc, char **argv)
{
    char buffer[BUFFER_SIZE];
    FILE *in = stdin;
    if (argc > 1) {
        in = fopen(argv[1], "r");
        if (!in) {
            fprintf(stderr, "Cannot open %s: %s\n", argv[1], strerror(errno));
            return 1;
        }
    }
    FILE *out = stdout;
    if (argc > 2) {
        out = fopen(argv[2], "w+");
        if (!out) {
            fprintf(stderr, "Cannot open %s: %s\n", argv[2], strerror(errno));
            fclose(in);
            return 1;
        }
    }
    unsigned long line = 0;
    unsigned long column = 0;
    unsigned long loop_lvl = 0;
    write_out(BFHEADER, TAPE_SIZE);
    while (fgets(buffer, BUFFER_SIZE, in)) {
        for (int i = 0; i < strlen(buffer); ++i) {
            switch(buffer[i]) {
                case '+':
                    write_out(BFADD); break;
                case '-':
                    write_out(BFSUB); break;
                case '<':
                    write_out(BFLFT); break;
                case '>':
                    write_out(BFRGT); break;
                case ',':
                    write_out(BFINP); break;
                case '.':
                    write_out(BFOUT); break;
                case '[':
                    write_out(BFSLP); ++loop_lvl; break;
                case ']':
                    write_out(BFELP); --loop_lvl; break;
                case '\n':
                    ++line; column = 0; break;
            }
            if (loop_lvl == ULONG_MAX) {
                fprintf(stderr, "Unexpected ']' at line %lu, column %lu\n", line, column);
                close_io();
                return 1;
            }
            ++column;
        }
    }
    write_out(BFFOOTER);
    if (loop_lvl > 0) {
        // unbalanced brackets
        fprintf(stderr, "Expected ']' at line %lu, column %lu\n", line, column);
        close_io();
        return 1;
    }
    close_io();
    return 0;
}
