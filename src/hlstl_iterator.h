#ifndef HLSTL_ITERATOR_H
#define HLSTL_ITERATOR_H

#include "hlstl_iterator_base.h"
namespace hl
{

// 尾部插入
template <typename Container>
class back_insert_iterator
{
protected:
    Container* pContainer_;

public:
    using container_type = Container;
    using iterator_category = output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    explicit back_insert_iterator(Container& container)
        : pContainer_(&container) {}

    back_insert_iterator<Container>& operator=(const typename Container::value_type& value)
    {
        pContainer_->push_back(value);
        return *this;
    }
    back_insert_iterator<Container>& operator*() { return *this; }
    back_insert_iterator<Container>& operator++() { return *this; }
    back_insert_iterator<Container>& operator++(int) { return *this; }
};

template <typename Container>
inline back_insert_iterator<Container> back_inserter(Container& container)
{
    return back_insert_iterator<Container>(container);
}

// 头部插入
template <typename Container>
class front_insert_iterator
{
protected:
    Container* pContainer_;

public:
    using container_type = Container;
    using iterator_category = output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    explicit front_insert_iterator(Container& container)
        : pContainer_(&container) {}

    front_insert_iterator<Container>& operator=(const typename Container::value_type& value)
    {
        pContainer_->push_front(value);
        return *this;
    }
    front_insert_iterator<Container>& operator*() { return *this; }
    front_insert_iterator<Container>& operator++() { return *this; }
    front_insert_iterator<Container>& operator++(int) { return *this; }
};

template <typename Container>
inline front_insert_iterator<Container> front_inserter(Container& container)
{
    return front_insert_iterator<Container>(container);
}

// 在iter位置连续插入
template <class Container>
class insert_iterator
{
protected:
    Container* pContainer_;
    typename Container::iterator iter_;

public:
    using container_type = Container;
    using iterator_category = output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    insert_iterator(Container& container, typename Container::iterator it)
        : pContainer_(&container), iter_(it) {}
    insert_iterator<Container>& operator=(const typename Container::value_type& value)
    {
        iter_ = pContainer_->insert(iter_, value);
        ++iter_;
        return *this;
    }
    insert_iterator<Container>& operator*() { return *this; }
    insert_iterator<Container>& operator++() { return *this; }
    insert_iterator<Container>& operator++(int) { return *this; }
};

template <typename Container, typename Iterator>
inline insert_iterator<Container> inserter(Container& container, Iterator iter)
{
    using Iter = typename Container::iterator;
    return insert_iterator<Container>(container, Iter(iter));
}

// reserve bidirectional iterator
// 反向 双向 迭代器
template <typename BidirectionalIterator,
          typename T,
          typename Reference = T&,
          typename Distance = ptrdiff_t>
class reverse_bidirectional_iterator
{
private:
    using Self = reverse_bidirectional_iterator<BidirectionalIterator, T, Reference, Distance>;

protected:
    BidirectionalIterator current_;

public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = Distance;
    using pointer = T*;
    using reference = Reference;

    reverse_bidirectional_iterator() {}
    explicit reverse_bidirectional_iterator(BidirectionalIterator iter) : current_(iter) {}
    // 这个接口看起来有些奇怪，干啥用的呢
    BidirectionalIterator base() const { return current_; }

    // bidirectional iterator 特性
    Reference operator*() const
    {
        BidirectionalIterator tmp = current_;
        return *--tmp;
    }
    pointer operator->() const { return &(operator*()); }

    Self& operator++()
    {
        --current_;
        return *this;
    }

    Self operator++(int)
    {
        Self tmp = *this;
        --current_;
        return tmp;
    }

    Self& operator--()
    {
        ++current_;
        return *this;
    }

    Self operator--(int)
    {
        Self tmp = *this;
        ++current_;
        return tmp;
    }

    bool operator==(Self& another) const
    {
        return this->current_ == another.current_;
    }
    bool operator!=(Self& another) const
    {
        return this->current_ != another.current_;
    }
};

// 为什么用全局重载而非友元函数？
// template <class _BiIter, class _Tp, class _Ref, class _Distance>
// inline bool operator==(
//     const reverse_bidirectional_iterator<_BiIter, _Tp, _Ref, _Distance>& __x,
//     const reverse_bidirectional_iterator<_BiIter, _Tp, _Ref, _Distance>& __y)
// {
//   return __x.base() == __y.base();
// }

template <typename Iterator>
class reverse_iterator
{
protected:
    Iterator current_;

public:
    using iterator_category = typename iterator_traits<Iterator>::iterator_category;
    using value_type = typename iterator_traits<Iterator>::value_type;
    using difference_type = typename iterator_traits<Iterator>::difference_type;
    using pointer = typename iterator_traits<Iterator>::pointer;
    using reference = typename iterator_traits<Iterator>::reference;

    using iterator_type = Iterator;
    using Self = reverse_iterator<Iterator>;

    reverse_iterator() {}
    explicit reverse_iterator(Iterator iter) : current_(iter){};
    reverse_iterator(const Self& it) : current_(it.current_) {}
    // reverse_iterator(const Self&& it) : current_(std::move(it.current_)) {}

    // 还有这种操作!
    template <class Iter>
    reverse_iterator(const reverse_iterator<Iter>& another)
        : current_(another.base()) {}

    iterator_type base() const { return current_; }
    reference operator*() const
    {
        iterator_type tmp = current_;
        return *(--tmp);
    }

    pointer operator->() const
    {
        return &(operator*());
    }

    Self& operator++()
    {
        --current_;
        return *this;
    }

    Self operator++(int)
    {
        Self tmp = *this;
        --current_;
        return tmp;
    }

    Self& operator--()
    {
        ++current_;
        return *this;
    }
    Self operator--(int)
    {
        Self tmp = *this;
        ++current_;
        return tmp;
    }

    Self operator+(difference_type n) const
    {
        return Self(current_ - n);
    }

    Self& operator+=(difference_type n) const
    {
        current_ -= n;
        return *this;
    }

    Self operator-(difference_type n) const
    {
        return Self(current_ + n);
    }

    Self& operator-=(difference_type n) const
    {
        current_ += n;
        return *this;
    }

    // interesting
    reference operator[](difference_type n) const
    {
        return *(*this + n);
    }
};

// 调用base
template <typename Iterator>
inline bool operator==(const reverse_iterator<Iterator>& lhs,
                       const reverse_iterator<Iterator>& rhs)
{
    return lhs.base() == rhs.base();
}

template <typename Iterator>
inline bool operator<(const reverse_iterator<Iterator>& lhs,
                      const reverse_iterator<Iterator>& rhs)
{
    return rhs.base() < lhs.base();
}

// 复用已有重载
template <typename Iterator>
inline bool operator!=(const reverse_iterator<Iterator>& lhs,
                       const reverse_iterator<Iterator>& rhs)
{
    return !(lhs == rhs);
}

// left > right <=> right < left
template <typename Iterator>
inline bool operator>(const reverse_iterator<Iterator>& lhs,
                      const reverse_iterator<Iterator>& rhs)
{
    return rhs < lhs;
}

template <typename Iterator>
inline bool operator<=(const reverse_iterator<Iterator>& lhs,
                       const reverse_iterator<Iterator>& rhs)
{
    return !(rhs < lhs);
}

template <typename Iterator>
inline bool operator>=(const reverse_iterator<Iterator>& lhs,
                       const reverse_iterator<Iterator>& rhs)
{
    return !(lhs < rhs);
}

} // namespace hl

#endif
