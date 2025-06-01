#ifndef __DEF_H__
#define __DEF_H__

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

typedef unsigned char uint8;
typedef unsigned int uint;
typedef long long int64;
typedef uintptr_t uintptr;
typedef size_t usize;
typedef uint32_t uint32;

#define ASSERT_NOT_NULL(ptr)                                                       \
    do {                                                                           \
        if ((ptr) == NULL) {                                                       \
            fprintf(stderr, "NULL_PTR_ERROR: '%s' is NULL at %s:%d in %s()\n", \
                    #ptr, __FILE__, __LINE__, __func__);                           \
            assert(0 && "Null Pointer Exception");                                 \
        }                                                                          \
    } while (0)

#define printerr(fmt, ...) \
        fprintf(stderr, "%s:%d :: %s " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define __PRINT_STOP__ \
        fprintf(stderr, "%s:%d (%s): PRINT STOP\n", __FILE__, __LINE__, __func__);

#endif