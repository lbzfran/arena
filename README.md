
# Arena Allocator

Standard Allocation Arena written in C,
packaged in an stb-style header file.

Unit test(s) available to compile and run using `make`.

# Rundown

arena.h
```c
/*Zero is Initialization.*/
Arena arena = {0};

/*Arena allocates its memory automatically.*/
unsigned int *x = PushStruct(&arena, unsigned int);

/*Sets value to 0.*/
ZeroStruct(x);

/*Or write out the non-macro equivalent.*/
/*The extra parameter '4' sets the alignment of the memory*/
/*For convenience, this is done automatically with PushStruct.*/
unsigned int *y = (unsigned int *)ArenaPush(&arena, sizeof(unsigned int), 4);

/*There's also a macro for the alignment setting.*/
// PushStructAlign, PushSize to PushSizeAlign, etc...
int **z = PushArrayAlign(&arena, int, 64, 8);

/* Free the arena. */
ArenaFree(&arena);
```

arena_linear.h (old implementation)
```c
/* Allocate the arena. */
Arena* arena = (Arena*)malloc(sizeof(Arena));
ArenaAlloc(local_arena, 1024); // 1 Kilobyte.

/* Get a pointer from the arena with specified size. */
unsigned int* x = (unsigned int *)ArenaPush(arena, sizeof(unsigned int));

/* You may also use this macro for convenience. */
unsigned int* y = PushStruct(arena, unsigned int);

/* Allocate Arrays using the arena. */
int **z = PushArray(arena, int, 64); // int array of size 64.
// equivalent to:
// int **z = (int **)ArenaPush(arena, sizeof(int)*64);

/* Set new allocations to 0 (memset). */
int *a = PushStructZero(arena, int);
int **b = PushArrayZero(arena, int, 12);

/* Free the arena. */
ArenaFree(arena);
```

# Note on header file versioning

This is my own standard for organizing my C files.

## Versioning in stb-style header files.

```
x.y
```

- x corresponds to major, *breaking* changes.
>   Current function signatures are changed; Key features
>   are changed.
- y corresponds to minor, nonbreaking changes.
>   Could be new features, or changes to existing functions
>   that does not change their current signatures.
>   Cleanup code. Rearranged, optimized, or added small changes
>   that doesn't necessarily change functionality.

## Additional info on versioning

If desired, the versioning can receive a letter prefix
to make its difference more apparent.
Ex. 'F-x.y' corresponding to its codebase versioning.

If in use, it is implied that the first, original version
uses the 'A-' prefix.
