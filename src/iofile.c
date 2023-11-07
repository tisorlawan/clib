#include "iofile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Based on https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer#:~:text=Here%20is%20what%20I%20would%20recommend.
 */
int file_read_all(FILE* in, char** data, size_t* len)
{
    size_t allocated = FILE_READ_ALL_CHUNK;
    size_t nread_total = 0;
    size_t nread = 0;

    *data = malloc(FILE_READ_ALL_CHUNK);
    if (data == NULL) {
        return FILE_READ_ALL_NOMEM;
    }

    while (!feof(in)) {
        int l = allocated - nread_total;

        nread = fread((*data) + nread_total, 1, l, in);
        if (nread) {
            nread_total += nread;
        } else if (ferror(in)) {
            free(*data);
            return FILE_READ_ALL_ERR_FREAD;
        }

        if (nread_total == allocated) {
            allocated += FILE_READ_ALL_CHUNK;
            char* new_data = realloc(*data, allocated);
            if (new_data == NULL) {
                free(*data);
                return FILE_READ_ALL_NOMEM;
            }
            *data = new_data;
        }
    }

    /* optimize the buffer size */
    char* new_data = realloc(*data, nread_total + 1);
    if (new_data == NULL) {
        free(*data);
        return FILE_READ_ALL_NOMEM;
    }
    *data = new_data;
    *len = nread_total;

    (*data)[nread_total] = '\0';

    return 0;
}

void read_file_all_example(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    char* data = NULL;
    size_t len;
    int status = file_read_all(fp, &data, &len);
    if (status != 0) {
        printf("error reading file with status %d\n", status);
    }

    free(data);
    fclose(fp);
}
