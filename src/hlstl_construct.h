#ifndef HLSTL_CONSTRUCT_H
#define HLSTL_CONSTRUCT_H
#include "hlstl_iterator_base.h"
#include "hlstl_type_traits.h"

#include <climits>
#include <cstddef>
#include <iostream>

namespace hl
{

namespace detail
{
template <typename T, typename... Args>
inline void __construct(T* p, Args&&... args)
{
    // std::cout << sizeof...(args) << std::endl;
    new (p) T(std::forward<Args>(args)...);
}

template <typename T>
inline void __destroy(T* p)
{
    p->~T();
}

template <typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
    for (; first != last; ++first)
        __destroy(&*first);
}

template <typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

template <typename ForwardIterator, typename T>
inline void __destory_iterator(ForwardIterator first, ForwardIterator last, T*)
{
    using has_trivial_destructor = typename __type_traits<T>::has_trivial_destructor;
    __destroy_aux(first, last, has_trivial_destructor());
}

inline void __destory_iterator(char*, char*) {}
inline void __destory_iterator(unsigned char*, unsigned char*) {}
inline void __destory_iterator(short*, short*) {}
inline void __destory_iterator(unsigned short*, unsigned short*) {}
inline void __destory_iterator(int*, int*) {}
inline void __destory_iterator(unsigned int*, unsigned int*) {}
inline void __destory_iterator(long*, long*) {}
inline void __destory_iterator(unsigned long*, unsigned long*) {}
inline void __destory_iterator(float*, float*) {}
inline void __destory_iterator(double*, double*) {}

} // namespace detail

template <typename T, typename... Args>
inline void construct(T* p, Args&&... args)
{
    detail::__construct(p, std::forward<Args>(args)...);
}

template <typename T>
inline void destroy(T* p)
{
    detail::__destroy(p);
}

// 将[first,last)范围内的所有对象释析构掉
template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
    detail::__destory_iterator(first, last, __VALUE_TYPE(first));
}

} // namespace hl

#endif
