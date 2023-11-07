#include "../src/vector.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct heap_allocated {
    char* name;
};

struct heap_allocated heap_allocated_new(const char* name, size_t name_len)
{
    struct heap_allocated h;
    h.name = malloc(name_len + 1);
    assert(h.name);
    strncpy(h.name, name, name_len + 1);
    h.name[name_len] = '\0';
    return h;
}

void heap_allocated_free(struct heap_allocated* h)
{
    free(h->name);
}

int main(void)
{
    size_t i;

    vector_init(names, struct heap_allocated, &heap_allocated_free);
    vector_init(nums, int, NULL);

    for (i = 0; i < 1000; i++) {
        char name[100] = { 0 };
        strncpy(name, "Tiso", 5);
        vector_push(&names, heap_allocated_new(name, strlen(name)));
        vector_push(&nums, i);
    }

    for (i = 0; i < vector_len(nums) && i < 20; i++) {
        printf("%lu => %d\n", i, nums[i]);
    }

    vector_free(nums);
    vector_free(names);

    return 0;
}
