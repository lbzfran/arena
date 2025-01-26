
#include "platform.h"

#include <sys/mman.h>

void* AllocateMemory(memory_index size)
{
    void* ptr = mmap(NULL, size,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    return(ptr);
}

void DeallocateMemory(void* ptr, memory_index size)
{
    munmap(ptr, size);
    ptr = NULL;
}
