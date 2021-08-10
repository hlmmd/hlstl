#ifndef HLSTL_ALLOC_H
#define HLSTL_ALLOC_H

namespace hl
{

template <class T, class Alloc>
class simple_alloc
{
public:
    // 配置 n 个元素
    static T* allocate(size_t n)
    {
        return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
    }
    static T* allocate(void)
    {
        return (T*)Alloc::allocate(sizeof(T));
    }
    static void deallocate(T* p, size_t n)
    {
        if (0 != n)
            Alloc::deallocate(p, n * sizeof(T));
    }
    static void deallocate(T* p)
    {
        Alloc::deallocate(p, sizeof(T));
    }
};

} // namespace hl

#endif
