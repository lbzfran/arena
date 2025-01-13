
# Arena Allocator

Standard Linear Arena Allocator written in c.

Unit test(s) available to compile and run using `make`.

# Rundown

```c
/* Allocate the arena. */
Arena* arena = (Arena*)malloc(sizeof(Arena));
ArenaAlloc(local_arena, 1024); // 1 Kilobyte.

/* Get a pointer from the arena with specified size. */
unsigned int x = (unsigned int*)ArenaPush(arena, sizeof(unsigned int));

/* You may also use this macro for convenience. */
unsigned int y = PushStruct(arena, unsigned int);

/* Allocate Arrays using the arena. */
int **z = PushArray(arena, int, 64); // int array of size 64.

/* Set new allocations to 0 (memset). */
int *a = PushStructZero(arena, int);
int **b = PushArrayZero(arena, int, 12);

/* Free the arena. */
ArenaFree(arena);
```
