/*
Copyright (c) 2025, Jiaming Meng

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __JM_TEMPLATE_DEFS_H__
#define __JM_TEMPLATE_DEFS_H__

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef unsigned char uint8;
typedef unsigned int uint;
typedef long long int64;
typedef uintptr_t uintptr;
typedef size_t usize;
typedef uint32_t uint32;

// shorter hand formats
typedef int32_t I32;
typedef long L32;
typedef int64_t I64;
typedef int I32;
typedef uint32_t U32;
typedef uint64_t U64;
typedef char I8; // this might ignore the existence of signed char
typedef int8_t S8; // signed char :)
typedef uint8_t U8;
typedef uintptr_t UPtr;
typedef size_t Sz;
typedef bool Bool;
typedef float F32;
typedef double F64;
typedef int16_t I16;
typedef uint16_t U16;
typedef void U0;
typedef void Void;

#define ensure(ptr)                                                           \
        if ((ptr) == NULL)                                                    \
        {                                                                     \
                fprintf(stderr, "NULL_PTR_ERROR: '%s' is NULL at %s:%d in %s()\n", \
                         #ptr, __FILE__, __LINE__, __func__);                 \
                exit(EXIT_FAILURE);                                           \
        }

// Printf but with a new line at the end
#define println(format, ...) printf(format "\n", ##__VA_ARGS__)

// Prints the target message to stderr with debug information including file name, line number, and function name
#define printerr(fmt, ...) \
        fprintf(stderr, "\n%s:%d :: %s " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#endif