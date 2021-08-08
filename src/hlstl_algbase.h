#ifndef HLSTL_ALGBASE_H
#define HLSTL_ALGBASE_H
#include "hlstl_iterator_base.h"
#include "hlstl_type_traits.h"
#include <glog/logging.h>
#include <string.h>
namespace hl
{

template <typename InputIter, typename OutputIter, typename Distance>
inline OutputIter __copy(InputIter first, InputIter last,
                         OutputIter result, input_iterator_tag, Distance*)
{
    for (; first != last; ++result, ++first)
    {
        *result = *first;
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

} // namespace hl

#endif