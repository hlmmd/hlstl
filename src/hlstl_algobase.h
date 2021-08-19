#ifndef HLSTL_ALGBASE_H
#define HLSTL_ALGBASE_H
#include "hlstl_iterator_base.h"
#include "hlstl_type_traits.h"
#include <glog/logging.h>
#include <string.h>
namespace hl
{

template <class T>
inline void swap(T& lhs, T& rhs)
{
    T tmp = lhs;
    lhs = rhs;
    rhs = tmp;
}

// min max
template <typename T>
inline const T& max(const T& lhs, const T& rhs)
{
    return lhs < rhs ? rhs : lhs;
}

template <typename T, typename Compare>
inline const T& max(const T& lhs, const T& rhs, const Compare& compare)
{
    return compare(lhs, rhs) ? rhs : lhs;
}

template <typename T>
inline const T& min(const T& lhs, const T& rhs)
{
    return lhs < rhs ? lhs : rhs;
}

template <typename T, typename Compare>
inline const T& min(const T& lhs, const T& rhs, const Compare& compare)
{
    return compare(lhs, rhs) ? lhs : rhs;
}

// copy TODO： move 版本
template <typename InputIter, typename OutputIter, typename Distance>
inline OutputIter __copy(InputIter first, InputIter last,
                         OutputIter result, input_iterator_tag, Distance*)
{
    for (; first != last; ++result, ++first)
    {
        *result = std::move(*first);
        // *result = *first;
    }
    return result;
}

// what the fuck
// template <typename RandomAccessIter, class OutputIter, typename Distance>
// inline OutputIter
// __copy(RandomAccessIter first, RandomAccessIter last,
//        OutputIter result, random_access_iterator_tag, Distance*)
// {
// }

// memmove for trivial type
template <typename T>
inline T* __copy_trivial(const T* first, const T* last, T* result)
{
    ::memmove(result, first, sizeof(T) * (last - first));
    return result + (last - first);
}

template <typename InputIter, typename OutputIter>
inline OutputIter __copy_aux2(InputIter first, InputIter last,
                              OutputIter result, __false_type)
{
    return __copy(first, last, result,
                  __ITERATOR_CATEGORY(first),
                  __DISTANCE_TYPE(first));
}

template <typename InputIter, typename OutputIter>
inline OutputIter __copy_aux2(InputIter first, InputIter last,
                              OutputIter result, __true_type)
{
    return __copy(first, last, result,
                  __ITERATOR_CATEGORY(first),
                  __DISTANCE_TYPE(first));
}

template <typename T>
inline T* __copy_aux2(T* first, T* second, T* result, __true_type)
{
    return __copy_trivial(first, second, result);
}

template <typename T>
inline T* __copy_aux2(const T* first, const T* second, T* result, __true_type)
{
    return __copy_trivial(first, second, result);
}

template <typename InputIter, typename OutputIter, typename T>
inline OutputIter __copy_aux(InputIter first, InputIter last, OutputIter result, T*)
{
    using IsTrivial = typename __type_traits<T>::has_trivial_assignment_operator;
    return __copy_aux2(first, last, result, IsTrivial());
}
template <typename InputIter, typename OutputIter>
inline OutputIter copy(InputIter first, InputIter last, OutputIter result)
{
    return __copy_aux(first, last, result, __VALUE_TYPE(first));
}

// copy_backward

template <typename InputIter, typename OutputIter>
inline OutputIter __copy_backward_aux2(InputIter first, InputIter last, OutputIter result)
{
    for (; last != first; --last, --result)
    {
        *(result - 1) = std::move(*(last - 1));
        // *(result - 1) = *(last - 1);
    }
    return result;
}

template <typename T>
inline T* __copy_backward_aux(const T* first, const T* last, const T* result)
{
    ::memmove(result - (last - first), first, (last - first));
    return result - (last - first);
}

template <typename InputIter, typename OutputIter, typename T>
inline OutputIter __copy_backward_aux(InputIter first, InputIter last, OutputIter result, T*)
{
    return __copy_backward_aux2(first, last, result);
}

template <typename InputIter, typename OutputIter>
inline OutputIter copy_backward(InputIter first, InputIter last, OutputIter result)
{
    return __copy_backward_aux(first, last, result, __VALUE_TYPE(first));
}

// fill
template <typename ForwardIter, typename T>
inline void fill(ForwardIter first, ForwardIter last, const T& value)
{
    for (; first != last; ++first)
        *first = value;
}

template <typename ForwardIter, typename T>
inline void fill(ForwardIter first, ForwardIter last, T* ptr = nullptr)
{
    for (; first != last; ++first)
        *first = T();
}

// 特化
template <>
inline void fill(unsigned char* first, unsigned char* last, const unsigned char& value)
{
    ::memset(first, value, last - first);
}

template <>
inline void fill(char* first, char* last, const char& value)
{
    ::memset(first, value, last - first);
}

// fill_n
template <typename ForwardIter, typename Size, typename T>
inline ForwardIter fill_n(ForwardIter first, Size n, const T& value)
{
    for (; n > 0; --n, ++first)
        *first = value;
    return first;
}

template <class Size>
inline unsigned char* fill_n(unsigned char* first, Size n, const unsigned char& value)
{
    fill(first, first + n, value);
    return first + n;
}

template <class Size>
inline char* fill_n(char* first, Size n, const char& value)
{
    fill(first, first + n, value);
    return first + n;
}

} // namespace hl

#endif
