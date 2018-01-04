#include "Memory.h"

Memory::Memory() : length{0}, items{nullptr}
{
}

Memory::Memory(int length) : length{length}, items{new Any[length]}
{
}

Memory::Memory(const Memory& other)
{
    length = other.length;
    items = new Any[length];
    for (i32 i = 0; i < length; i++)
    {
        items[i] = other.items[i];
    }
}

Memory& Memory::operator=(const Memory& other)
{
    length = other.length;
    items = new Any[length];
    for (i32 i = 0; i < length; i++)
    {
        items[i] = other.items[i];
    }
    return *this;
}
Memory::Memory(Memory&& other)
{
    length = other.length;
    items = other.items;
    other.items = nullptr;
}

Memory& Memory::operator=(Memory&& other)
{
    length = other.length;
    items = other.items;
    other.items = nullptr;
    return *this;
}

Any& Memory::operator[](i32 index)
{
    return items[index];
}

Memory::~Memory()
{
    delete[] items;
}

i32 Memory::Length()
{
    return length;
}
