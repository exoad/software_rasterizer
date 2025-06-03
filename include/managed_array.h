#ifndef __MANAGED_ARRAY_H__
#define __MANAGED_ARRAY_H__

#include <stdbool.h>

#include "allocator.h"

/// @brief A dynamic array that relies on an arena to manage memory
typedef struct JM_ManagedArray {
    void* data;
    usize count;
    usize capacity;
    usize elementSize;
    JM_Arena* arena;
} JM_ManagedArray;
/// @brief Initializes a Managed array with the supplied parameters
static void jm_managed_array_create(JM_ManagedArray* array, const usize elementSize, JM_Arena* arena)
{
    array->data = NULL;
    array->count = 0;
    array->capacity = 0;
    array->elementSize = elementSize;
    array->arena = arena;
}
/// @brief Inserts a new element to the end of the dynamic growing array
static bool jm_managed_array_push(JM_ManagedArray* array, const void* element)
{
    if(array->count >= array->capacity)
    {
        const usize capacity = (array->capacity == 0) ? 16 : array->capacity * 2;
        void* newPtr = jm_arena_alloc(array->arena, capacity * array->elementSize);
        if(newPtr == NULL)
        {
            printerr("%s", "Failed to allocate the dynamic array using the arena.");
            return false;
        }
        if(array->data)
        {
            memcpy(newPtr, array->data, array->count * array->elementSize);
        }
        array->data = newPtr;
        array->capacity = capacity;
    }
    if(element)
    {
        memcpy((char*) array->data + (array->count * array->elementSize), element, array->elementSize);
    }
    array->count++;
    return true;
}
/// @brief Releases the resources used by the managed array
static void jm_managed_array_free(JM_ManagedArray* array)
{
    array->data = NULL;
    array->count = 0;
    array->capacity = 0;
}

#endif