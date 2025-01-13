#ifndef ALLOC_H
#define ALLOC_H

#include "base.h"

typedef struct memory_arena {
    memory_index size;
    uint8* base;
    memory_index pos; // aka used memory idx

    uint32 tempCount;
} Arena;

typedef struct memory_arena_temp {
    Arena* arena;
    memory_index pos;
} ArenaTemp;

void ArenaAlloc(Arena* arena, memory_index);
void ArenaFree(Arena*);

void* ArenaPush(Arena*, memory_index);
void* ArenaPushZero(Arena*, memory_index);

// NOTE(liam): helper macros
#define PushArray(arena, t, c) (t*)ArenaPush((arena),sizeof(t)*(c))
#define PushStruct(arena, t) PushArray(arena, t, 1)
#define PushArrayZero(arena, t, c) (t*)ArenaPushZero((arena),sizeof(t)*(c))
#define PushStructZero(arena, t) PushArrayZero(arena, t, 1)

void ArenaPop(Arena*, memory_index);
uint64 ArenaGetPos(Arena*);

void ArenaSetPos(Arena*, memory_index);
void ArenaClear(Arena*);


ArenaTemp ArenaTempBegin(Arena*); // grabs arena's position
void ArenaTempEnd(ArenaTemp);     // restores arena's position
void ArenaTempCheck(Arena*);

ArenaTemp GetScratch(Arena*);
#define FreeScratch(t) ArenaTempEnd(t)

#endif
