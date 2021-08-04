#ifndef HLSTL_ITERATOR_BASE_H
#define HLSTL_ITERATOR_BASE_H
#include <cstddef>
namespace hl
{
// iterator_category 五种迭代器类型标记
struct input_iterator_tag
{
};
struct output_iterator_tag
{
};
struct forward_iterator_tag : public input_iterator_tag
{
};
struct bidirectional_iterator_tag : public forward_iterator_tag
{
};
struct random_access_iterator_tag : public bidirectional_iterator_tag
{
};

template <class T, class Distance>
struct input_iterator
{
    using iterator_category = input_iterator_tag;
    using value_type = T;
    using difference_type = Distance;
    using pointer = T*;
    using reference = T&;
};

template <class T, class Distance>
struct output_iterator
{
    using iterator_category = output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
};

template <class T, class Distance>
struct forward_iterator
{
    using iterator_category = forward_iterator_tag;
    using value_type = T;
    using difference_type = Distance;
    using pointer = T*;
    using reference = T&;
};

template <class T, class Distance>
struct bidirectional_iterator
{
    using iterator_category = bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = Distance;
    using pointer = T*;
    using reference = T&;
};

template <class T, class Distance>
struct random_access_iterator
{
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = Distance;
    using pointer = T*;
    using reference = T&;
};

template <typename Category, typename T, typename Distance = ptrdiff_t,
          typename Pointer = T*, typename Reference = T&>
struct iterator
{
    using iterator_category = Category;
    using value_type = T;
    using difference_type = Distance;
    using pointer = Pointer;
    using reference = Reference;
};

template <class Iterator>
struct iterator_traits
{
    using iterator_category = typename Iterator::iterator_category;
    using value_type = typename Iterator::value_type;
    using difference_type = typename Iterator::difference_type;
    using pointer = typename Iterator::pointer;
    using reference = typename Iterator::reference;
};

// 针对原生指针(native pointer)而设计的 traits 偏特化版
template <class Iterator>
struct iterator_traits<Iterator*>
{
    using iterator_category = random_access_iterator_tag;
    using value_type = Iterator;
    using difference_type = ptrdiff_t;
    using pointer = Iterator*;
    using reference = Iterator&;
};

template <typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category
__iterator_category(const Iterator&)
{
    using Category = typename iterator_traits<Iterator>::iterator_category;
    return Category();
}

template <typename Iterator>
inline typename iterator_traits<Iterator>::difference_type*
__difference_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

template <typename Iterator>
inline typename iterator_traits<Iterator>::value_type*
__value_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

template <typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator& it)
{
    return __iterator_category(it);
}

template <typename Iterator>
inline typename iterator_traits<Iterator>::difference_type*
difference_type(const Iterator& it)
{
    return __difference_type(it);
}

template <typename Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator& it)
{
    return __value_type(it);
}

#define __ITERATOR_CATEGORY(__i) __iterator_category(__i)
#define __DISTANCE_TYPE(__i) __distance_type(__i)
#define __VALUE_TYPE(__i) __value_type(__i)

// distance
template <typename InputIteraotr>
inline typename iterator_traits<InputIteraotr>::difference_type
__distance(InputIteraotr first, InputIteraotr last, input_iterator_tag)
{
    typename iterator_traits<InputIteraotr>::difference_type n = 0;
    while (first != last)
    {
        first++;
        n++;
    }
    return n;
}

template <typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
{
    return last - first;
}

template <typename Iterator>
inline typename iterator_traits<Iterator>::difference_type
distance(Iterator first, Iterator last)
{
    return __distance(first, last, iterator_category(first));
}

template <typename InputIterator, typename Distance>
inline void __advance(InputIterator& it, Distance n, input_iterator_tag)
{
    while (n--)
        ++it;
}

template <typename BidirectionalIterator, typename Distance>
inline void __advance(BidirectionalIterator& it, Distance n, bidirectional_iterator_tag)
{
    if (n >= 0)
    {
        while (n--)
            ++it;
    }
    else
    {
        while (n++)
            --it;
    }
}

template <typename RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator& it, Distance n,
                      random_access_iterator_tag)
{
    it += n;
}

template <typename Iterator, class Distance>
inline void advance(Iterator& it, Distance n)
{
    __advance(it, n, iterator_category(it));
}

} // namespace hl

#endif
