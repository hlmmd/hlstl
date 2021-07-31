#ifndef HLSTL_ALLOCATOR_H
#define HLSTL_ALLOCATOR_H
#include <climits>
#include <cstddef>
#include <iostream>
namespace hl
{

template <typename T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    // void rebind() {}
    Allocator() = default;
    ~Allocator() = default;

    static pointer Allocate(size_type n, const void* p = nullptr)
    {
        pointer ret = (T*)(::operator new(sizeof(T) * n));
        if (ret == nullptr)
        {
            std::cout << "out of memory" << std::endl;
            exit(-1);
        }
        return ret;
    }
    static void DeAllocate(pointer p)
    {
        ::operator delete(p);
    }

    static void DeAllocate(pointer p, size_t n)
    {
        ::operator delete(p, n);
    }

private:
    pointer address(reference x)
    {
        return pointer(&x);
    }
    const_pointer const_address(const_reference x) const
    {
        return const_pointer(&x);
    }

    size_type init_page_size()
    {
        return std::max(size_type(1), size_type(4096 / sizeof(T)));
    }
    size_type max_size() const
    {
        return std::max(size_type(1), size_type(UINT_MAX / sizeof(T)));
    }
};

// template <class T, class Alloc>
// class SimpleAlloc
// {
// public:
//     static T* Allocate(size_t n)
//     {
//         return !n ? nullptr : (T*)Alloc::allocate(n * sizeof(T));
//     }
//     static T* Allocate()
//     {
//         return (T*)Alloc::allocate(sizeof(T));
//     }
// };

} // namespace hl

#endif
