#ifndef IOFILE_H
#define IOFILE_H
#include <stdio.h>

typedef int IOFILE_STATUS;

#ifndef IOFILE_READ_CHUNK
#define IOFILE_READ_CHUNK 2097152 /* 2 MB */
#endif

#define IOFILE_STATUS_OK 0 /* Success */
#define IOFILE_STATUS_ERR_FREAD -1 /* Stream error (fread error) */
#define IOFILE_STATUS_NOMEM -2 /* Out of memory */

/*
FILE *f = fopen("path", "r");

char *buf = NULL;
size_t len = 0;

IOFILE_STATUS x = iofile_read_all(f, &buf, &len);
if (x != IOFILE_STATUS_OK) {
    fprintf(stderr, "error reading file\n");
}

free(buf);
fclose(f);
*/
IOFILE_STATUS iofile_read_all(FILE* in, char** data, size_t* len);

#endif /* IOFILE_H */

#ifdef IOFILE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Based on https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer#:~:text=Here%20is%20what%20I%20would%20recommend.
 */
int iofile_read_all(FILE* in, char** data, size_t* len)
{
    size_t allocated = IOFILE_READ_CHUNK;
    size_t nread_total = 0;
    size_t nread = 0;

    *data = malloc(IOFILE_READ_CHUNK);
    if (data == NULL) {
        return IOFILE_STATUS_NOMEM;
    }

    while (!feof(in)) {
        int l = allocated - nread_total;

        nread = fread((*data) + nread_total, 1, l, in);
        if (nread) {
            nread_total += nread;
        } else if (ferror(in)) {
            free(*data);
            return IOFILE_STATUS_ERR_FREAD;
        }

        if (nread_total == allocated) {
            allocated += IOFILE_READ_CHUNK;
            char* new_data = realloc(*data, allocated);
            if (new_data == NULL) {
                free(*data);
                return IOFILE_STATUS_NOMEM;
            }
            *data = new_data;
        }
    }

    /* optimize the buffer size */
    char* new_data = realloc(*data, nread_total + 1);
    if (new_data == NULL) {
        free(*data);
        return IOFILE_STATUS_NOMEM;
    }
    *data = new_data;
    *len = nread_total;

    (*data)[nread_total] = '\0';

    return IOFILE_STATUS_OK;
}

#endif /* IOFILE_IMPLEMENTATION */
