#ifndef __IOFILE__
#define __IOFILE__
#include <stdio.h>

#ifndef FILE_READ_ALL_CHUNK
#define FILE_READ_ALL_CHUNK 2097152 /* 2 MB */
#endif

#define FILE_READ_ALL_OK 0 /* Success */
#define FILE_READ_ALL_ERR_FREAD -1 /* Stream error (fread error) */
#define FILE_READ_ALL_NOMEM -2 /* Out of memory */

/*
    TODO: add better documentation

    FILE* fp = fopen("myfile", "r");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    char* data = NULL;
    size_t len;
    int status = file_read_all(fp, &data, &len);

    free(data);
    fclose(fp);
*/

extern int file_read_all(FILE* in, char** data, size_t* len);

#endif /* __IOFILE__ */
