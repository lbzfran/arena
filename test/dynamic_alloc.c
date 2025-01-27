#define ARENA_IMPLEMENTATION
#include "../arena.h"

int main(void)
{
    Arena arena = {0};
    ArenaSetMinimumBlockSize(&arena, Kilobytes(1));

    uint16* t1_a = PushStruct(&arena, uint16, 0);
    printf("arena blocks BEFORE: %d\n", arena.blockCount);
    printf("base ptr of arena: %p\n", (void*)arena.base);
    printf("%p = %d\n", (void*)t1_a, *t1_a);
    for (size_t i = 0; i < 1024; i++)
    {
        uint16* t1_b = PushStruct(&arena, uint16, 0);
        *t1_b = i;
        if ((void*)&arena.base == (void*)&t1_b) {
            printf("SAME PTR FOUND %p = %d\n", (void*)&t1_b, *t1_b);
        }
    }
    printf("%p = %d\n", (void*)t1_a, *t1_a);

    printf("arena blocks AFTER: %d\n", arena.blockCount);
    printf("base ptr of arena: %p\n", (void*)arena.base);

    ArenaFree(arena);

    //TODO(liam): implement deallocation caused by Pushing into arena dynamically.
    return 0;
}
