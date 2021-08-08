#ifndef HLSTL_UNINITIALIZED_H
#define HLSTL_UNINITIALIZED_H

#include "hlstl_algbase.h"
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

} // namespace hl

#endif