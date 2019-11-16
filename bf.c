#include <stdio.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 256
#define TAPE_SIZE 30000

int
main(int argc, char **argv)
{
    char t[TAPE_SIZE], *p = t;
    char buffer[BUFFER_SIZE];
    FILE *fp = stdin;
    if (argc > 1) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            fprintf(stderr, "Cannot open %s: %s\n", argv[1], strerror(errno));
            return 1;
        }
    }
    while (fgets(buffer, BUFFER_SIZE, fp)) {
        printf("%s", buffer);
    }
    return 0;
}
