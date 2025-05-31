#ifndef __DEF_H__
#define __DEF_H__

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

typedef unsigned char uint8;
typedef unsigned int uint;
typedef long long int64;

#define ASSERT_NOT_NULL(ptr)                                                       \
    do {                                                                           \
        if ((ptr) == NULL) {                                                       \
            fprintf(stderr, "NULL_PTR_ERROR: '%s' is NULL at %s:%d in function %s()\n", \
                    #ptr, __FILE__, __LINE__, __func__);                           \
            assert(0 && "Null Pointer Exception");                                 \
        }                                                                          \
    } while (0)

#endif