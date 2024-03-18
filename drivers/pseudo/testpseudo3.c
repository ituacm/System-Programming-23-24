#include <stdio.h>

#define MAX_BUF 80

int main(int argc, char *argv[]) {
    FILE *fp = NULL;
    char buf[MAX_BUF];

    fp = fopen("/dev/pseudo", "r");
    fread(buf, 1, 1, fp);
    buf[1] = '\0';
    printf("%s\n", buf);
    fclose(fp);
    return 0;
}
