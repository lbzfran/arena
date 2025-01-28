/*
 * ---------------
 * Liam Bagabag
 * Version: 2.1
 * Requires: none (inline)
 * ---------------
 */
#ifndef ARENA_H
#define ARENA_H

// COMPATIBILITY LAYER
// NOTE(liam): define 'GIVEMEMALLOC' if you want to allocate memory yourself.
// afterwards, you have to define both 'a_alloc' and 'a_free', or
// it will fallback to using the std lib malloc implementation.
/*#define GIVEMEMALLOC*/

#include "platform.h"

#include <stdalign.h>

typedef struct memory_arena_footer {
    uint8* base;
    memory_index size;
    memory_index pos;
    memory_index padding;
} ArenaFooter;

typedef struct memory_arena {
    uint8* base;
    memory_index size;
    memory_index pos; // aka used memory idx

    memory_index minimumBlockSize;

    uint32 blockCount;
    uint32 tempCount;
} Arena;

typedef struct memory_arena_temp {
    Arena* arena;
    uint8* base;
    memory_index pos;
    memory_index padding;
} ArenaTemp;

/*void ArenaFree(Arena);*/

void* ArenaPush(Arena*, memory_index, memory_index);
void* ArenaCopy(memory_index, void*, Arena*);
ArenaFooter* GetFooter(Arena* arena);

memory_index ArenaGetEffectiveSize(Arena* arena, memory_index sizeInit, memory_index alignment);
memory_index ArenaGetAlignmentOffset(Arena* arena, memory_index alignment);
memory_index ArenaGetRemainingSize(Arena* arena, memory_index alignment);

void ArenaFreeCurrentBlock(Arena* arena);

// NOTE(liam): auto-aligned Push Instructions.
#define PushArray(arena, t, c) (t*)ArenaPush((arena),sizeof(t)*(c), alignof(t))
#define PushStruct(arena, t) PushArray(arena, t, 1)
#define PushSize(arena, s) ArenaPush((arena), (s), alignof(s))
#define PushCopy(arena, s, src) (ArenaCopy(s, src, ArenaPush(arena, s, alignof(s)))

// NOTE(liam): Set Alignment Manually.
#define PushArrayAlign(arena, t, c, ...) (t*)ArenaPush((arena),sizeof(t)*(c), ## __VA_ARGS__)
#define PushStructAlign(arena, t, ...) PushArray(arena, t, ## __VA_ARGS__)
#define PushSizeAlign(arena, s, ...) ArenaPush((arena), (s), ## __VA_ARGS__)
#define PushCopyAlign(arena, s, src, ...) (ArenaCopy(s, src, ArenaPush(arena, s, ## __VA_ARGS__))
void ArenaFillZero(memory_index size, void *ptr);

/*void ArenaPop(Arena*, memory_index);*/
uint64 ArenaGetPos(Arena*);

void ArenaSetMinimumBlockSize(Arena* arena, memory_index minimumBlockSize);
void ArenaSetPos(Arena*, memory_index);
void ArenaClear(Arena*);
#define ArenaFree(arena) ArenaClear(arena);

void SubArena(Arena* subArena, Arena* arena, memory_index size, memory_index alignment);

ArenaTemp ArenaTempBegin(Arena*);
void ArenaTempEnd(ArenaTemp);
void ArenaTempCheck(Arena*);

ArenaTemp GetScratch(Arena*);
#define FreeScratch(t) ArenaTempEnd(t)

#define ZeroStruct(in) ArenaFillZero(sizeof(in), &(in))
#define ZeroArray(n, ptr) ArenaFillZero((n)*sizeof((ptr)[0]), (ptr))

inline void
ArenaFillZero(memory_index size, void *ptr) // effectively memcpy
{
    uint8* byte = (uint8*) ptr;
    while (size--) {
        *byte++ = 0;
    }
}

// NOTE(liam): does not inherently allocate memory.
// Must be performed as user sees fit before call.
/*inline void*/
/*ArenaInit(Arena* arena, memory_index size, void* base_addr)*/
/*{*/
/*    arena->size = size;*/
/*    arena->base = (uint8 *)base_addr;*/
/*    arena->pos = 0;*/
/*    arena->tempCount = 0;*/
/*    arena->minimumBlockSize = 0;*/
/*}*/
inline void
ArenaSetMinimumBlockSize(Arena* arena, memory_index minimumBlockSize)
{
    arena->minimumBlockSize = minimumBlockSize;
}

/*inline void*/
/*ArenaInitBlock(Arena* arena, memory_index minimumBlockSize)*/
/*{*/
/*    arena->size = 0;*/
/*    arena->base = 0;*/
/*    arena->pos  = 0;*/
/*    arena->tempCount = 0;*/
/*    arena->minimumBlockSize = minimumBlockSize;*/
/*}*/

// NOTE(liam): convenience arena wrapper that uses
// malloc implementation defined by user (or stdlib).
/*inline Arena**/
/*ArenaMalloc(memory_index size)*/
/*{*/
/*    Arena* arena = (Arena*)a_malloc(size);*/
/*    ArenaInit(arena, size, a_malloc(size));*/
/**/
/*    return(arena);*/
/*}*/

// NOTE(liam): this can be potentially skipped if you're
// using other forms of memory allocation (VirtualAlloc, mmap).
// A good way to check if you need it is if you used the
// ArenaMalloc function for allocation.
/*inline void*/
/*ArenaFree(Arena* arena)*/
/*{*/
/*    ArenaClear(arena);*/
/*    *arena = {0};*/
/*}*/

inline memory_index
ArenaGetAlignmentOffset(Arena* arena, memory_index alignment)
{
    memory_index alignmentOffset = 0;

    memory_index resPointer = (memory_index)arena->base + arena->pos;
    memory_index alignmentMask = alignment - 1;
    if (resPointer & alignmentMask)
    {
        alignmentOffset = alignment - (resPointer & alignmentMask);
    }

    return(alignmentOffset);
}

inline memory_index
ArenaGetRemainingSize(Arena* arena, memory_index alignment)
{
    memory_index res = arena->size - (arena->pos + ArenaGetAlignmentOffset(arena, alignment));
    return(res);
}

inline memory_index
ArenaGetEffectiveSize(Arena* arena, memory_index sizeInit, memory_index alignment)
{
    memory_index size = sizeInit;

    memory_index alignmentOffset = ArenaGetAlignmentOffset(arena, alignment);
    size += alignmentOffset;

    return(size);
}

inline bool32
ArenaCanStoreSize(Arena* arena, memory_index sizeInit, memory_index alignment)
{
    if (!alignment) alignment = 4;

    memory_index size = ArenaGetEffectiveSize(arena, sizeInit, alignment);
    bool32 res = (arena->pos + size <= arena->size);

    return(res);
}

inline ArenaFooter*
GetFooter(Arena* arena)
{
    ArenaFooter *res = (ArenaFooter*)(arena->base + arena->size);

    return(res);
}

inline void*
ArenaPush(Arena* arena, memory_index sizeInit, memory_index alignment)
{
    if (!alignment) alignment = 4;

    //NOTE(liam): rounds allocation up to set align properly.
    memory_index size = ArenaGetEffectiveSize(arena, sizeInit, alignment);

    /*Assert(arena->pos + size < arena->size, "requested alloc size exceeds arena size.")*/
    if ((arena->pos + size) > arena->size)
    {
        // NOTE(liam): if min block size was never set, set it.
        if (!arena->minimumBlockSize)
        {
            // TODO(liam): tune block sizing
            arena->minimumBlockSize = Megabytes(1); // 1024 * 1024
        }

        ArenaFooter save = {0};
        save.base = arena->base;
        save.size = arena->size;
        save.pos = arena->pos;

        // NOTE(liam): base should automatically align after allocating again.
        size = sizeInit;
        memory_index blockSize = Max(size + sizeof(struct memory_arena_footer), arena->minimumBlockSize);
        arena->size = blockSize - sizeof(struct memory_arena_footer);
        arena->base = (uint8*)AllocateMemory(blockSize);
        arena->pos = 0;
        arena->blockCount++;

        ArenaFooter* footer = GetFooter(arena);
        *footer = save;
    }
    Assert((arena->pos + size) <= arena->size, "new allocation of dynamic arena somehow failed...");

    memory_index alignmentOffset = ArenaGetAlignmentOffset(arena, alignment);
    void* res = (void*)(arena->base + arena->pos - alignmentOffset);
    arena->pos += size;

    Assert(size >= sizeInit, "requested alloc exceeds arena size after alignment.");

    return(res);
}

inline void
SubArena(Arena* subArena, Arena* arena, memory_index size, memory_index alignment)
{
    if (!alignment) alignment = 16;

    subArena->size = size;
    subArena->base = (uint8*)PushSizeAlign(arena, size, alignment);
    subArena->pos = 0;
    subArena->tempCount = 0;
}


//TODO(liam): finish implementation
/*void ArenaPop(Arena *arena, memory_index size) {*/
/*}*/

inline uint64
ArenaGetPos(Arena *arena)
{
    return(arena->pos);
}

inline void
ArenaSetPos(Arena *arena, memory_index pos)
{
    Assert(pos <= arena->pos, "Setting position beyond current arena allocation.");
    arena->pos = pos;
}


// NOTE(liam): effectively resets the Arena.
inline void
ArenaClear(Arena *arena)
{
    while (arena->blockCount)
    {
        ArenaFreeCurrentBlock(arena);
    }
}

// NOTE(liam): temporary memory.
inline ArenaTemp
ArenaTempBegin(Arena *arena)
{
    ArenaTemp res;

    res.arena = arena;
    res.base = arena->base;
    res.pos = arena->pos;

    arena->tempCount++;

    return(res);
}

inline void
ArenaFreeCurrentBlock(Arena* arena)
{
    void* freedBlock = arena->base;
    memory_index freedBlockSize = arena->size;

    ArenaFooter* footer = GetFooter(arena);

    arena->base = footer->base;
    arena->size = footer->size;
    arena->pos  = footer->pos;

    DeallocateMemory(freedBlock, freedBlockSize);

    arena->blockCount--;
}

inline void
ArenaTempEnd(ArenaTemp temp)
{
    Arena* arena = temp.arena;

    while(arena->base != temp.base)
    {
        ArenaFreeCurrentBlock(arena);
    }

    Assert(arena->pos >= temp.pos, "Arena position is less than temporary memory's position. Likely user-coded error.");
    arena->pos = temp.pos;

    Assert(arena->tempCount > 0, "Attempt to decrement Arena's temporary memory count when it is already 0.");
    arena->tempCount--;
}

// NOTE(liam): should call after finishing temp use.
// need to make sure all temps are accounted for before
// resuming allocations.
inline void
ArenaTempCheck(Arena* arena)
{
    Assert(arena->tempCount == 0, "")
}

inline ArenaTemp
GetScratch(Arena* arena)
{
    Assert(arena->pos + sizeof(ArenaTemp) <= arena->size, "requested temp alloc exceeds arena size.");

    ArenaTemp temp = ArenaTempBegin(arena);
    return temp;
}

#endif //ARENA_H
