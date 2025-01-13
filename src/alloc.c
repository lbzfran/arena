
#include <stdlib.h>
#include <string.h>
#include "alloc.h"
#include "base.h"

// NOTE(liam): does not inherently allocate memory.
// Must be performed as user sees fit before call.
void ArenaAlloc(Arena* arena, memory_index size) {
    arena->size = size;
    arena->pos = 0;
    arena->base = malloc(size);
    arena->tempCount = 0;
}

void ArenaFree(Arena* arena) {
    if (arena) {
        free(arena->base);
        free(arena);
    }
}

void* ArenaPush(Arena* arena, memory_index size) {
    Assert(arena->pos + size < arena->size, "requested alloc size exceeds arena size.")
    void* res = arena->base + arena->pos;
    arena->pos += size;

    return(res);
}

void* ArenaPushZero(Arena* arena, memory_index size) {
    Assert(arena->pos + size < arena->size, "requested alloc size exceeds arena size.")
    void* res = arena->base + arena->pos;
    arena->pos += size;

    memset(res, 0, size);

    return(res);
}

// pop some bytes off the 'stack' - the way to free
void ArenaPop(Arena *arena, memory_index size) {
    //TODO(liam): fix this
    Assert(arena->pos - size >= 0, "");

}

// get the # of bytes currently allocated.
uint64 ArenaGetPos(Arena *arena) {
    return(arena->pos);
}

// also some useful popping helpers:
void ArenaSetPos(Arena *arena, memory_index pos) {
    Assert(pos <= arena->pos, "Setting position beyond current arena allocation.");
    arena->pos = pos;
}

// effectively resets the Arena.
void ArenaClear(Arena *arena) {
    arena->pos = 0;
    arena->tempCount = 0;
}

// NOTE(liam): temporary memory
ArenaTemp ArenaTempBegin(Arena *arena) {
    ArenaTemp res;

    res.arena = arena;
    res.pos = arena->pos;

    arena->tempCount++;

    return(res);
}

void ArenaTempEnd(ArenaTemp temp) {
    Arena* arena = temp.arena;

    Assert(arena->pos >= temp.pos, "Arena position is less than temporary memory's position. Likely user-coded error.");
    arena->pos = temp.pos;

    Assert(arena->tempCount > 0, "Attempt to decrement Arena's temporary memory count when it is already 0.");
    arena->tempCount--;
}

// NOTE(liam): should call after temp use.
// need to make sure all temps are accounted for before
// resuming allocations.
void ArenaTempCheck(Arena* arena) {
    Assert(arena->tempCount == 0, "")
}

ArenaTemp GetScratch(Arena* arena) {
    Assert(arena->pos + sizeof(ArenaTemp) <= arena->size, "requested temp alloc exceeds arena size.");

    ArenaTemp temp = ArenaTempBegin(arena);
    return temp;
}
