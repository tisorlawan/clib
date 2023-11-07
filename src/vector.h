#ifndef VECTOR_H
#define VECTOR_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define VECTOR_INITIAL_CAPACITY 8

/* Based on and inspired by
 * https://github.com/eteran/c-vector/blob/master/cvector.ho
 */

typedef struct vector_metadata {
    size_t size;
    size_t capacity;
    void (*elem_dtor)(void*);
} vec_meta_t;

#define vector_init(name, type, dtor)                                          \
    type* name;                                                                \
    do {                                                                       \
        struct vector_metadata* _meta_;                                        \
        _meta_ = malloc(                                                       \
            sizeof(vec_meta_t) + (VECTOR_INITIAL_CAPACITY * sizeof(type)));    \
        assert(_meta_);                                                        \
        _meta_->size = 0;                                                      \
        _meta_->capacity = VECTOR_INITIAL_CAPACITY;                            \
        _meta_->elem_dtor = (void (*)(void*))dtor;                             \
        name = metadata_to_vector(_meta_);                                     \
    } while (0);

#define vector_push(vec, elem)                                                 \
    do {                                                                       \
        vec_meta_t* meta;                                                      \
        size_t new_capacity;                                                   \
        meta = vector_to_metadata(*vec);                                       \
        new_capacity = meta->capacity * 2;                                     \
                                                                               \
        if (meta->capacity <= meta->size) {                                    \
            size_t alloc_size                                                  \
                = new_capacity * sizeof(**vec) + sizeof(vec_meta_t);           \
            vec_meta_t* new_meta;                                              \
            new_meta = realloc(meta, alloc_size);                              \
            assert(new_meta);                                                  \
                                                                               \
            *vec = metadata_to_vector(new_meta);                               \
            (*vec)[new_meta->size++] = elem;                                   \
            meta = vector_to_metadata(*vec);                                   \
            meta->capacity = new_capacity;                                     \
        } else {                                                               \
            (*vec)[meta->size++] = elem;                                       \
        }                                                                      \
    } while (0);

#define vector_to_metadata(vec) (&((vec_meta_t*)(vec))[-1])
#define metadata_to_vector(meta) (void*)(int*)(meta + 1)

#define vector_len(vec) vector_to_metadata(vec)->size
#define vector_capacity(vec) vector_to_metadata(vec)->capacity

#define vector_pop(vec)                                                        \
    do {                                                                       \
        vec_meta_t* meta = vector_to_metadata(vec);                            \
        if (meta->size > 0) {                                                  \
            meta->size--;                                                      \
        }                                                                      \
    } while (0);

#define vector_free(vec)                                                       \
    do {                                                                       \
        vec_meta_t* meta = vector_to_metadata(vec);                            \
        size_t i;                                                              \
        for (i = 0; i < meta->size; i++) {                                     \
            if (meta->elem_dtor) {                                             \
                meta->elem_dtor(&vec[i]);                                      \
            }                                                                  \
        }                                                                      \
        free(meta);                                                            \
    } while (0);

#endif /* VECTOR_H */
