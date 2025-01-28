/*
 * ---------------
 * Liam Bagabag
 * Version: 2.0.1
 * Requires: platform.h
 * ---------------
 */
#ifndef ARENA_H
#define ARENA_H

// NOTE(liam): define 'ARENA_LETMEALLOCATE' if you want to allocate memory
// yourself. afterwards, you have to define both 'a_alloc' and 'a_free', or
// it will fallback to using the std lib malloc implementation.
// NOTE(liam): if you don't define it, you will need to either implement
// the allocators based on their signature, or include platform_<win/linux>.c
// in compilation.
#define ARENA_LETMEALLOCATE
/*#define a_alloc ...*/
/*#define a_free ...*/

// COMPATIBILITY LAYER
# include <stdio.h>
# include <stdint.h>
# include <string.h>
typedef uint8_t     uint8;  // unsigned char
typedef uint16_t    uint16; // unsigned int
typedef uint32_t    uint32; // unsigned long int
typedef uint64_t    uint64; // unsigned long long int

typedef int8_t      int8;  // signed char
typedef int16_t     int16; // signed int
typedef int32_t     int32; // signed long int
typedef int64_t     int64; // signed long long int

typedef size_t      memory_index;
typedef int32       bool32;

# define Min(a, b)  ((a) < (b) ? (a) : (b))
# define Max(a, b)  ((a) > (b) ? (a) : (b))
# define ArrayCount(a) (sizeof(a)/sizeof(*(a)))

# define Kilobytes(V) ((V)*1024LL)
# define Megabytes(V) (Kilobytes(V)*1024LL)
# define Gigabytes(V) (Megabytes(V)*1024LL)
# define Terabytes(V) (Gigabytes(V)*1024LL)

// DEBUG START
# define ENABLE_DEBUG
# ifdef  ENABLE_DEBUG
#  include <stdlib.h>
// NOTE(liam): force exit if expression 'c' is false. Basically program-breaking error.
#  define Assert(c,msg) if (!(c)) { fprintf(stderr, "[-] <ASSERTION ERROR> at line %d:  %s.\n", __LINE__, #msg); exit(1); }
// NOTE(liam): force exit program. basically code should never reach this point.
#  define Throw(msg) { fprintf(stderr, "[-] <THROW> at line %d: %s.\n", __LINE__, #msg); exit(1); }
# else
#  define ASSERT(c,msg)
#  define Throw(msg) { exit(1); }
# endif
// DEBUG END

/*struct Platform {*/
/*    void* (*AllocateMemory)(memory_index);*/
/*    void (*DeallocateMemory)(void*, memory_index);*/
/*};*/

#ifndef ARENA_LETMEALLOCATE
void* AllocateMemory(memory_index);
void DeallocateMemory(void*, memory_index);
#else
# if !defined(a_alloc) || !defined(a_free)
#  include <stdlib.h>
#  define a_alloc malloc
#  define a_free free
# endif
#
# define AllocateMemory(s) a_alloc((s))
# define DeallocateMemory(ptr, s) a_free((ptr))
#endif

typedef struct memory_arena_footer {
    uint8* base;
    memory_index size;
    memory_index pos;
    memory_index padding;
} ArenaFooter;

typedef struct memory_arena {
    memory_index size;
    uint8* base;
    memory_index pos; // aka used memory idx

    memory_index minimumBlockSize;

    uint32 blockCount;
    uint32 tempCount;
} Arena;

typedef struct memory_arena_temp {
    Arena* arena;
    uint8* base;
    memory_index pos;
} ArenaTemp;

/*void ArenaInit(Arena*, memory_index, void*);*/
/*void ArenaInitBlock(Arena*, memory_index);*/
/*Arena* ArenaMalloc(memory_index size);*/
void ArenaFree(Arena);

void* ArenaPush(Arena*, memory_index, memory_index);
void* ArenaCopy(memory_index, void*, Arena*);
ArenaFooter* GetFooter(Arena* arena);

memory_index ArenaGetEffectiveSize(Arena* arena, memory_index sizeInit, memory_index alignment);
memory_index ArenaGetAlignmentOffset(Arena* arena, memory_index alignment);
memory_index ArenaGetRemainingSize(Arena* arena, memory_index alignment);

void ArenaFreeLastBlock(Arena* arena);

// NOTE(liam): helper macros
#define PushArray(arena, t, c, ...) (t*)ArenaPush((arena),sizeof(t)*(c), ## __VA_ARGS__)
#define PushStruct(arena, t, ...) PushArray(arena, t, 1, ## __VA_ARGS__)
#define PushSize(arena, s, ...) ArenaPush((arena), (s), ## __VA_ARGS__)
#define PushCopy(arena, s, src, ...) (ArenaCopy(s, src, ArenaPush(arena, s, ## __VA_ARGS__))
/*#define PushArrayZero(arena, t, c) (t*)ArenaPushZero((arena),sizeof(t)*(c))*/
/*#define PushStructZero(arena, t) PushArrayZero(arena, t, 1)*/
void ArenaFillZero(memory_index size, void *ptr);

void ArenaPop(Arena*, memory_index);
uint64 ArenaGetPos(Arena*);

void ArenaSetMinimumBlockSize(Arena* arena, memory_index minimumBlockSize);
void ArenaSetPos(Arena*, memory_index);
void ArenaClear(Arena*);

void SubArena(Arena* subArena, Arena* arena, memory_index size, memory_index alignment);

ArenaTemp ArenaTempBegin(Arena*); // grabs arena's position
void ArenaTempEnd(ArenaTemp);     // restores arena's position
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
inline void
ArenaFree(Arena arena)
{
    DeallocateMemory(arena.base, arena.size);
}

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

    return (alignmentOffset);
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

    memory_index size = ArenaGetEffectiveSize(arena, sizeInit, alignment);

    /*Assert(arena->pos + size < arena->size, "requested alloc size exceeds arena size.")*/
    if ((arena->pos + size) > arena->size)
    {
        if (!arena->minimumBlockSize)
        {
            // TODO(liam): tune block sizing
            arena->minimumBlockSize = Megabytes(1); // 1024 * 1024
        }

        ArenaFooter save;
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
    subArena->base = (uint8*)PushSize(arena, size, alignment);
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
    while (arena->blockCount > 0)
    {
        ArenaFreeLastBlock(arena);
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
ArenaFreeLastBlock(Arena* arena)
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
        ArenaFreeLastBlock(arena);
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

#endif // ARENA_IMPLEMENTATION
