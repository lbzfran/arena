#ifndef ARENA_MEMORY_H
#define ARENA_MEMORY_H

// NOTE(liam): Memory Allocation.

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

// TODO(liam): figure out how to further abstract this;
// see handmade hero: find platform layer, platform_allocate_memory, etc.
/*#define PLATFORM_ALLOCATE_MEMORY(fn) fn()*/
/*struct Platform {*/
/*    void* (*AllocateMemory)(memory_index);*/
/*    void (*DeallocateMemory)(void*, memory_index);*/
/*};*/

#ifndef ARENA_USEMALLOC
// NOTE(liam): likely do not modify this.
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

#endif //ARENA_MEMORY_H

