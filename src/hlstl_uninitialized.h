#ifndef HLSTL_UNINITIALIZED_H
#define HLSTL_UNINITIALIZED_H
#include "hlstl_algobase.h"
#include "hlstl_construct.h"
#include "hlstl_iterator_base.h"
#include "hlstl_type_traits.h"

namespace hl
{

// POD type，直接copy，不需要构造
template <typename InputIter, typename ForwardIter>
inline ForwardIter
__uninitialized_copy_aux(InputIter first, InputIter last, ForwardIter result, __true_type)
{
    return copy(first, last, result);
}

// 非POD type，需要构造
template <typename InputIter, typename ForwardIter>
inline ForwardIter
__uninitialized_copy_aux(InputIter first, InputIter last, ForwardIter result, __false_type)
{
    ForwardIter cur = result;
    // 如果construct的时候发生异常咋整
    for (; first != last; ++first, ++cur)
    {
        construct(&(*cur), *first);
    }
    return cur;
}

template <typename InputIter, typename ForwardIter, typename T>
inline ForwardIter
__uninitialized_copy(InputIter first, InputIter last, ForwardIter result, T*)
{
    using is_pod_type = typename __type_traits<T>::is_POD_type;
    return __uninitialized_copy_aux(first, last, result, is_pod_type());
}

template <typename InputIter, typename ForwardIter>
inline ForwardIter
uninitialized_copy(InputIter first, InputIter last, ForwardIter result)
{
    return __uninitialized_copy(first, last, result, __VALUE_TYPE(result));
}

// TODO uninitialized_copy_n
// what the fuck?

template <typename ForwardIter, typename T>
inline void __uninitialized_fill_aux(
    ForwardIter first, ForwardIter last, T&& value, __true_type)
{
    fill(first, last, std::forward<T>(value));
}

template <typename ForwardIter, typename T>
inline void __uninitialized_fill_aux(
    ForwardIter first, ForwardIter last, T&& value, __false_type)
{
    ForwardIter cur = first;
    for (; cur != last; ++cur)
        construct(&(*cur), std::forward<T>(value));
}

template <typename ForwardIter, typename T, typename T2>
inline void __uninitialized_fill(ForwardIter first,
                                 ForwardIter last,
                                 T&& value, T2*)
{
    using isPodType = typename __type_traits<T2>::is_POD_type;
    __uninitialized_fill_aux(first, last, std::forward<T>(value), isPodType());
}

template <typename ForwardIter, typename T>
inline void uninitialized_fill(ForwardIter first,
                               ForwardIter last,
                               T&& value)
{
    __uninitialized_fill(first, last, std::forward<T>(value), __VALUE_TYPE(first));
}

template <typename ForwardIter, typename T>
inline void uninitialized_fill(ForwardIter first,
                               ForwardIter last,
                               T* ptr = nullptr)
{
    ForwardIter cur = first;
    for (; cur != last; ++cur)
        construct(&(*cur));
}

template <typename ForwardIter, typename Size, typename T>
inline ForwardIter
__uninitialized_fill_n_aux(ForwardIter first, Size n, T&& value, __true_type)
{
    return fill_n(first, n, std::forward<T>(value));
}

template <typename ForwardIter, typename Size, typename T>
inline ForwardIter
__uninitialized_fill_n_aux(ForwardIter first, Size n, T&& value, __false_type)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        construct(&(*cur), std::forward<T>(value));
    return cur;
}

template <typename ForwardIter, typename Size, typename T, typename T2>
inline ForwardIter
__uninitialized_fill_n(ForwardIter first, Size n, T&& value, T2*)
{
    using isPodType = typename __type_traits<T2>::is_POD_type;
    return __uninitialized_fill_n_aux(first, n, std::forward<T>(value), isPodType());
}

template <typename ForwardIter, typename Size, typename T>
inline ForwardIter
uninitialized_fill_n(ForwardIter first, Size n, T&& value)
{
    return __uninitialized_fill_n(first, n, std::forward<T>(value), __VALUE_TYPE(first));
}

template <typename ForwardIter, typename Size, typename T>
inline ForwardIter
uninitialized_fill_n(ForwardIter first, Size n, T* ptr = nullptr)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        construct(&(*cur));
    return cur;
}

} // namespace hl

#endif
