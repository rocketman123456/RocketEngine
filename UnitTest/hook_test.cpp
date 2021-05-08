/*
    Rocketman
    2021-05-07
*/
#include <stdio.h>
#include <stdlib.h>

#if defined(RK_MACOS) || defined(RK_LINUX)
#include <dlfcn.h>

void* malloc(size_t size) {
    void* (*real_malloc)(size_t);
    real_malloc = (void* (*) (size_t))dlsym(RTLD_NEXT, "malloc");

    fprintf(stderr, "allocating %lu bytes\n", (unsigned long)size);
    /* Do your stuff here */

    return real_malloc(size);
}

void free(void* ptr) {
    void (*real_free)(void*);
    real_free = (void (*) (void*))dlsym(RTLD_NEXT, "free");

    fprintf(stderr, "free pointer address :%lu\n", (unsigned long)ptr);
    /* Do your stuff here */

    real_free(ptr);
}

#elif defined(RK_WINDOWS)
#include <malloc.h>


#endif

int main() {
    int* int_ptr = (int*)malloc(16);
    free(int_ptr);

    return 0;
}