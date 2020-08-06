#pragma once

#include <cstdlib>
#include <cstdint>

struct Block
{
    uint32_t start;
    uint32_t end;
    Block* prev;
    Block* next;

    Block(uint32_t _start, uint32_t _end, Block* _prev, Block* _next)
    {
        start = _start;
        end = _end;
        prev = _prev;
        next = _next;
    }

    inline uint32_t len() const { return end - start; }
};

//template <class T>
class Allocator
{
private:
    void* _pool;
    Block* _start = nullptr;

    // TODO: use this to either grow the heap, or throw an error
    uint32_t _size;

    uint32_t alloc_space(uint32_t size);

    template <class T>
    void free_space(T* ptr, uint32_t size);
public:
    Allocator(int size = 1024 * 1024 /* 1MB */);
    ~Allocator();

    template <class T, class... Args>
    T* alloc(Args&&... args);

    template <class T>
    T* alloc_size(uint32_t size);

    template <class T>
    void release(T* ptr);
};

template <class T>
void Allocator::free_space(T* ptr, uint32_t size)
{
    if (!_start) return;

    uint32_t ofs = reinterpret_cast<uint8_t*>(ptr) - reinterpret_cast<uint8_t*>(_pool);

    // Find the block
    Block* curr = _start;
    while (curr->next != nullptr) {
        if (curr->start <= ofs)
            break;
    }

    // Start of block
    if (ofs == curr->start) {
        curr->start += size;

        // Block is empty
        if (curr->len() == 0)
            delete curr;
    }

    // End of block
    else if (ofs + size == curr->end) {
        curr->end -= size;

        if (curr->len() == 0)
            delete curr;
    }

    // Middle of block
    else {
        Block* split = new Block(ofs + size, curr->end, curr, curr->next);
 
        if (curr->next)
            curr->next->prev = split;

        curr->next = split;
        curr->end = ofs;
    }
}

template <class T, class... Args>
T* Allocator::alloc(Args&&... args)
{
    T* ptr = alloc_size<T>(sizeof(T));
    return new (ptr) T(args...);
}

template <class T>
T* Allocator::alloc_size(uint32_t size)
{
    uint32_t ofs = alloc_space(sizeof(T) * size);
    return static_cast<T*>((void*)((uint8_t*)_pool + ofs));
}

template <class T>
void Allocator::release(T* ptr)
{
    free_space<T>(ptr, sizeof(T));
    ptr->~T();
    ptr = nullptr;
}

#ifdef JA_IMPL
uint32_t Allocator::alloc_space(uint32_t size)
{
    // First allocation
    if (_start == nullptr) {
        _start = new Block(0, size, nullptr, nullptr);
        return 0;
    }

    // Start at the beginning
    Block* curr = _start;

    // Go until we're at the last block if we need to
    while (curr->next != nullptr) {
        // We have room on this node
        if (curr->end + size <= curr->next->start)
            break;

        // Keep looking
        curr = curr->next;
    }

    uint32_t ofs = curr->end;
    curr->end += size;

    // These blocks meet, we can join them
    if (curr->next && curr->next->start == curr->end) {
        Block* next = curr->next;

        curr->end = next->end;
        if (next->next) {
            curr->next->prev = curr;
        }
        curr->next = next->next;

        delete next;
    }

    return ofs;
}

Allocator::Allocator(int size)
{
    _pool = malloc(size);
    _size = size;
}

Allocator::~Allocator()
{
    free(_pool);
    Block* blk = _start;
    if (blk) {
        while (blk->next != nullptr) {
            Block* tmp = blk;
            blk = blk->next;
            delete tmp;
        }
        delete blk;
    }
}
#endif