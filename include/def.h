#ifndef __DEF_H__
#define __DEF_H__

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef unsigned char uint8;
typedef unsigned int uint;
typedef long long int64;
typedef uintptr_t uintptr;
typedef size_t usize;
typedef uint32_t uint32;

#define ASSERT_NOT_NULL(ptr)                                                       \
    do {                                                                           \
        if ((ptr) == NULL) {                                                       \
            fprintf(stderr, "NULL_PTR_ERROR: '%s' is NULL at %s:%d in function %s()\n", \
                    #ptr, __FILE__, __LINE__, __func__);                           \
            assert(0 && "Null Pointer Exception");                                 \
        }                                                                          \
    } while (0)


#endif