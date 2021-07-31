#ifndef HLSTL_CONSTRUCT_H
#define HLSTL_CONSTRUCT_H
#include <climits>
#include <cstddef>
#include <iostream>
namespace hl
{

namespace detail
{
template <typename T, typename... Args>
inline void __Construct(T* p, const Args&... args)
{
    // std::cout << sizeof...(args) << std::endl;
    new (p) T(args...);
}

template <typename T>
inline void __Destroy(T* p)
{
    // TODO: POD type
    p->~T();
}

} // namespace detail

template <typename T, typename... Args>
inline void Construct(T* p, const Args&... args)
{
    detail::__Construct(p, args...);
}

template <typename T>
inline void Destroy(T* p)
{
    detail::__Destroy(p);
}

} // namespace hl

#endif