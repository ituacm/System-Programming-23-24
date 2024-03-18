#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *fp = NULL;
    
    fp = fopen("/dev/pseudo", "r");
    fclose(fp);
    return 0;
}
