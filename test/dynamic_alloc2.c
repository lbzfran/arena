#define ARENA_IMPLEMENTATION
#include "../old_arena.h"

int main(void)
{
    Arena* arena = ArenaMalloc(Kilobytes(1));
    /*ArenaSetMinimumBlockSize(&arena, Kilobytes(1));*/

    uint16* t1_a = PushStruct(arena, uint16);
    *t1_a = 1112;
    /*printf("arena blocks BEFORE: %d\n", arena.blockCount);*/
    printf("ptr of arena: %p\n", (void*)(arena->base + arena->pos));
    printf("a start: %p = %d\n", (void*)t1_a, *t1_a);
    /*printf("%p = %d\n", (void*)t1_a, *t1_a);*/
    for (size_t i = 0; i < 512; i++)
    {
        uint16* t1_b = PushStruct(arena, uint16);
        *t1_b = i;
        if ((void*)&t1_a == (void*)&t1_b) {
            printf("SAME PTR FOUND %p = %d\n", (void*)&t1_b, *t1_b);
        }
        if (i == 127) printf("%p\n", (void*)t1_b);
    }
    printf("a after mass alloc: %p = %d\n", (void*)t1_a, *t1_a);
    uint16* t1_c = PushStruct(arena, uint16);

    printf("a after alloc: %p = %d\n", (void*)t1_a, *t1_a);
    printf("c: %p = %d\n", (void*)t1_c, *t1_c);

    /*printf("arena blocks AFTER: %d\n", arena.blockCount);*/
    printf("ptr of arena: %p\n", (void*)(arena->base + arena->pos));

    ArenaFree(arena);

    return 0;
}
