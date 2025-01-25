
# Arena Allocator

Standard Linear Arena Allocator written in c,
packaged in an stb-style header file.

Unit test(s) available to compile and run using `make`.

# Rundown

```c
/* Allocate the arena. */
Arena* arena = (Arena*)malloc(sizeof(Arena));
ArenaAlloc(local_arena, 1024); // 1 Kilobyte.

/* Get a pointer from the arena with specified size. */
unsigned int x = (unsigned int *)ArenaPush(arena, sizeof(unsigned int));

/* You may also use this macro for convenience. */
unsigned int y = PushStruct(arena, unsigned int);

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
x.y.z
```

- x corresponds to major, *breaking* changes.
>   Current function signatures are changed; Key features
>   are changed.
- y corresponds to major, *nonbreaking* changes.
>   Could be new features, or changes to existing functions
>   that does not change their current signatures.
- z corresponds to minor changes.
>    Cleanup code. Rearranged, optimized, or added small changes
>    that doesn't necessarily change functionality.

If I change the files between codebases, I shouldn't
need to synchronize them if only 'z' changes have been made.
x and y however, will require synchronizing.

'x' changes can be done specifically for the codebase
it is on (to make it work better there), and in that case,
a manual diff must be done to sync only the changes that
needs to be synchronized.

Also, when making changes, it is not necessary to increment
the version with every change-- instead, synchronize the
increment with actual pushes to the codebase repo.

## Additional info on versioning

If desired, the versioning can receive a letter prefix
to make its difference more apparent.
Ex. 'F-x.y.z' corresponding to its codebase versioning.

If in use, it is implied that the first, original version
uses the 'A-' prefix.
