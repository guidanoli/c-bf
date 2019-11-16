#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define BUFFER_SIZE 256

#ifndef TAPE_SIZE
    #define TAPE_SIZE 30000
#endif

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

#define BF_HDR "#include <stdio.h>\nint t[%lu],*p=t;\nint main(){\n"
#define BF_ADD "++*p;"
#define BF_SUB "--*p;"
#define BF_LFT "--p;"
#define BF_RGT "++p;"
#define BF_INP "*p=getchar();"
#define BF_OUT "putchar(*p);"
#define BF_SLP "while(*p){"
#define BF_ELP "}"
#define BF_FTR "return 0;}"

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
    write_out(BF_HDR, TAPE_SIZE);
    while (fgets(buffer, BUFFER_SIZE, in)) {
        for (int i = 0; i < strlen(buffer); ++i) {
            switch(buffer[i]) {
                case '+':
                    write_out(BF_ADD); break;
                case '-':
                    write_out(BF_SUB); break;
                case '<':
                    write_out(BF_LFT); break;
                case '>':
                    write_out(BF_RGT); break;
                case ',':
                    write_out(BF_INP); break;
                case '.':
                    write_out(BF_OUT); break;
                case '[':
                    write_out(BF_SLP); ++loop_lvl; break;
                case ']':
                    write_out(BF_ELP); --loop_lvl; break;
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
    write_out(BF_FTR);
    if (loop_lvl > 0) {
        fprintf(stderr, "Expected ']' at line %lu, column %lu\n", line, column);
        close_io();
        return 1;
    }
    close_io();
    return 0;
}
