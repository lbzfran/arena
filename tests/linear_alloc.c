
#define ARENA_IMPLEMENTATION

#include <stdio.h>
#include "../src/arena_linear.h"

#define AND &&
#define OR ||
#define NOT !
/*void CheckExpression(const char* prompt, bool expression) {*/
/*    printf("TEST: %s. %s\n", prompt, expression ? "OK" : "FAIL");*/
/*}*/

#define CheckExpr(prompt, expression) printf("TEST: %s. %s\n", (prompt), (expression) ? "OK" : "FAIL")
#define PrintAddr(x) printf("%s = %p.\n", #x, (void*)(x))

int main(void) {
    printf("Testing my custom Arena Allocator.\n");
    CheckExpr("Byte Check", Kilobytes(1) == 1024 AND Megabytes(1) == 1048576);

    Arena* local_arena = ArenaMalloc(Kilobytes(2));

    CheckExpr("Arena Allocation", local_arena);

    int16* t1_a = PushStruct(local_arena, int16);
    int16* t1_b = PushStruct(local_arena, int16);

    PrintAddr(&t1_a);
    PrintAddr(&t1_b);


    CheckExpr("Initial Arena Allocation", NOT (*t1_a));
    CheckExpr("Initial Zeroed Arena Allocation", (*t1_b) == 0);

    (*t1_b) = 14;
    CheckExpr("Assignment to Arena Allocations", (*t1_b == 14));
    CheckExpr("Arena Memory Alignment during Assignment with same type", (*t1_a != 14));

    PrintAddr(&t1_a);
    PrintAddr(&t1_b);


    /*printf("allocated after assign: %d\n", (*x));*/
    int32* t2_a = PushStruct(local_arena, int32);
    int16* t2_b = PushStruct(local_arena, int16);
    int8* t2_c = PushStruct(local_arena, int8);
    int8* t2_d = PushStruct(local_arena, int8);
    int16* t2_e = PushStruct(local_arena, int16);
    int8* t2_f = PushStruct(local_arena, int8);
    int16* t2_g = PushStruct(local_arena, int16);
    int8* t2_h = PushStruct(local_arena, int8);
    int8* t2_i = PushStruct(local_arena, int8);

    CheckExpr("Multiple Arena Allocations with different types", local_arena->pos == 19);

    *(t2_a) = 1;
    *(t2_b) = 2;
    *(t2_c) = 3;
    *(t2_d) = 4;
    *(t2_e) = 5;
    *(t2_f) = 6;
    *(t2_g) = 7;
    *(t2_h) = 8;
    *(t2_i) = 9;

    CheckExpr("Arena Memory Alignment to Multiple Assignment with different types", \
            ((*t2_a) == 1 AND (*t2_b) == 2 AND (*t2_c) == 3 AND \
             (*t2_d) == 4 AND (*t2_e) == 5 AND (*t2_f) == 6 AND \
             (*t2_g) == 7 AND (*t2_h) == 8 AND (*t2_i) == 9)
            );

    /* testing scratch */
    memory_index t3_pos_before = local_arena->pos;
    ArenaTemp cow = GetScratch(local_arena);

    PushStruct(local_arena, int64);
    PushStruct(local_arena, int64);
    PushStruct(local_arena, int32);
    PushStruct(local_arena, int64);
    PushStruct(local_arena, int32);
    PushStruct(local_arena, int64);
    PushStruct(local_arena, int32);

    FreeScratch(cow);
    CheckExpr("Recover Position after Allocating and Freeing Scratch Arena", local_arena->pos == t3_pos_before);
    /* end scratch */

    ArenaFree(local_arena);
    return 0;
}
