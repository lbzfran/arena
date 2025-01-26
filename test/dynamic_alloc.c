#define ARENA_IMPLEMENTATION
#include "../arena.h"

int main(void)
{
    Arena arena = {};
    ArenaInitBlock(&arena, 0);

    int8* t1_a = PushStruct(&arena, int8, 0);
    *t1_a = 19;

    printf("%d\n", *t1_a);

    //TODO(liam): implement deallocation caused by Pushing into arena dynamically.
    return 0;
}
