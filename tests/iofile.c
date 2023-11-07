#include <stdio.h>
#include <stdlib.h>
#include "../src/iofile.h"

int main(void)
{
    int status;
    char* data = NULL;
    FILE* fp;
    size_t len;
    const char* fpath;

    fpath = "Makefile";
    printf("Opening '%s'\n", fpath);
    fp = fopen(fpath, "r");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    status = file_read_all(fp, &data, &len);
    printf("Status = %d\n", status);
    printf("%s\n", data);

    free(data);
    fclose(fp);

    return 0;
}
