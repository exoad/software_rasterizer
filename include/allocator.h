#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "def.h"

#define JM_ARENA_DEFAULT_BLOCK_SIZE (4096)
// power of 2 alignment
#define JM_ARENA_ALIGN_UP(p, align) (((UPtr)(p) + ((align) - 1)) & ~((UPtr)((align) - 1)))

typedef struct JM_ArenaBlock
{
    struct JM_ArenaBlock* next;
    Sz capacity;
    Sz offset;
    char data[];
} JM_ArenaBlock;

typedef struct JM_Arena
{
    JM_ArenaBlock* blocks;
    Sz blockSize;
} JM_Arena;

/// @brief Initialize the arena, blockSize is 0 by default
/// @param arena
/// @param blockSize
static U0 jm_arena_init(JM_Arena* arena, const Sz blockSize)
{
    arena->blocks = NULL;
    arena->blockSize = blockSize > 0 ? blockSize : JM_ARENA_DEFAULT_BLOCK_SIZE;
}
/// @brief Allocate a new block within the arena with teh specified minimum capacity
/// @param minCapacity
/// @return pointer to the block
static JM_ArenaBlock* jm_arena_create_block(const Sz minCapacity)
{
    const Sz capacity = minCapacity > JM_ARENA_DEFAULT_BLOCK_SIZE ? minCapacity : JM_ARENA_DEFAULT_BLOCK_SIZE;
    JM_ArenaBlock* block = malloc(sizeof(JM_ArenaBlock) + capacity);
    if(!block)
    {
        printerr("Failed to allocate a block in the arena with minCapacity %lld", minCapacity);
        return NULL;
    }
    block->next = NULL;
    block->capacity = capacity;
    block->offset = 0;
    return block;
}
/// @brief Allocation with alignment
/// @param arena
/// @param size
/// @param align
/// @return
static U0* jm_arena_alloc_aligned(JM_Arena* arena, const Sz size, const Sz align)
{
    ensure(arena);
    if(align & (align - 1))
    {
        printerr("%s", "Failed to allocate because the alignment is not a power of 2");
        return NULL;
    }
    JM_ArenaBlock* block = arena->blocks;
    if(block)
    {
        const UPtr raw = (UPtr) &block->data[block->offset];
        const UPtr aligned = JM_ARENA_ALIGN_UP(raw, align);
        const Sz adjust = aligned - raw;
        if(block->offset + adjust + size <= block->capacity)
        {
            U0* res = (U0*) aligned;
            block->offset += adjust + size;
            return res;
        }
    }
    const Sz required = size + align; // prepare to allocate this new block
    block = jm_arena_create_block(required);
    if(!block)
    {
        printerr("%s", "Failed to allocate a new arena block while trying to allocate.");
        return NULL;
    }
    block->next = arena->blocks;
    arena->blocks = block;
    const UPtr raw = (UPtr) &block->data[0];
    const UPtr aligned = JM_ARENA_ALIGN_UP(raw, align);
    const Sz adjust = aligned - raw;
    block->offset = adjust + size;
    return (U0*) aligned;
}
/// @brief Allocation with default alignment
/// @param arena
/// @param size
/// @return
static inline U0* jm_arena_alloc(JM_Arena* arena, const Sz size)
{
    return jm_arena_alloc_aligned(arena, size, _Alignof(max_align_t));
}
/// @brief Frees all blocks in the arena and prepares it for reuse.
/// @param arena
static U0 jm_arena_clean(const JM_Arena* arena)
{
    if(arena->blocks)
    {
        arena->blocks->offset = 0;
        JM_ArenaBlock* block = arena->blocks->next;
        arena->blocks->next = NULL;
        while(block)
        {
            JM_ArenaBlock* next = block->next;
            free(block);
            block = next;
        }
    }
}
/// @brief Frees all resources in the arena (destructive in the naming)
/// @param arena
static U0 jm_arena_destroy(JM_Arena* arena)
{
    JM_ArenaBlock* block = arena->blocks;
    while(block)
    {
        JM_ArenaBlock* next = block->next;
        free(block);
        block = next;
    }
    arena->blocks = NULL;
}

#endif