#define ARENA_IMPLEMENTATION
#include "../arena.h"

// TODO(liam): some alignment issue going on where pointers are getting
// overwritten somehow. need to look into it.

int main(void)
{
    Arena arena = {0};
    ArenaSetMinimumBlockSize(&arena, Kilobytes(1));

    uint16* t1_a = PushStruct(&arena, uint16, 8);
    *t1_a = 1112;
    printf("arena blocks BEFORE: %d\n", arena.blockCount);
    printf("ptr of arena: %p\n", (void*)(arena.base + arena.pos));
    printf("a start: %p = %d\n", (void*)t1_a, *t1_a);
    /*printf("%p = %d\n", (void*)t1_a, *t1_a);*/
    for (size_t i = 0; i < 128; i++)
    {
        uint16* t1_b = PushStruct(&arena, uint16, 0);
        *t1_b = i;
        if ((void*)&t1_a == (void*)&t1_b) {
            printf("SAME PTR FOUND %p = %d\n", (void*)&t1_b, *t1_b);
        }
        if (i == 127) printf("127 reached: %p\n", (void*)t1_b);
    }
    printf("a after mass alloc: %p = %d\n", (void*)t1_a, *t1_a);
    uint16* t1_c = PushStruct(&arena, uint16, 8);

    printf("a after alloc: %p = %d\n", (void*)t1_a, *t1_a);
    printf("c: %p = %d\n", (void*)t1_c, *t1_c);

    printf("arena blocks AFTER: %d\n", arena.blockCount);
    printf("ptr of arena: %p\n", (void*)(arena.base + arena.pos));

    ArenaFree(arena);

    //TODO(liam): implement deallocation caused by Pushing into arena dynamically.
    return 0;
}
