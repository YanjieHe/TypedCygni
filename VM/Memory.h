#ifndef MEMORY_H
#define MEMORY_H
#include "Any.h"
#include <stdint.h>
using i32 = int32_t;

class Memory
{
public:
    Memory();
    Memory(int length);
    Memory(const Memory& other);
    Memory& operator=(const Memory& other);
    Memory(Memory&& other);
    Memory& operator=(Memory&& other);
    Any& operator[](i32 index);
    ~Memory();
    i32 Length();

private:
    int length;
    Any* items;
};

#endif // MEMORY_H
