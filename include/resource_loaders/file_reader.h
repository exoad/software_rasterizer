#ifndef __FILE_READER_H__
#define __FILE_READER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "managed_array.h"
#include "utils.h"
#include "allocator.h"

// define the function prototypes for reading a file line by line and returning it as a managed array

JM_ManagedArray* jm_files_readline(const char* filename, JM_Arena* arena);

#endif