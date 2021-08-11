#ifndef HLSTL_VECTOR_H
#define HLSTL_VECTOR_H

#include "hlstl_alloc.h"
#include "hlstl_allocator.h"
#include "hlstl_iterator.h"
#include "hlstl_type_traits.h"

namespace hl
{

template <typename T, typename Alloc>
class VectorBase
{
public:
    using allocator_type = Alloc;
    allocator_type get_allocator() const { return allocator_type(); }

    VectorBase(const Alloc&) : start_(nullptr), finish_(nullptr), end_of_storage_(nullptr)
    {
    }

    VectorBase(size_t n, const Alloc&)
    {
        start_ = this->allocate(n);
        finish_ = start_;
        end_of_storage_ = start_ + n;
    }

    ~VectorBase()
    {
        this->deallocate(start_, end_of_storage_ - start_);
    }

protected:
    T* start_;
    T* finish_;
    T* end_of_storage_;
    using data_allocator_ = simple_alloc<T, Alloc>;
    T* allocate(size_t n)
    {
        return data_allocator_::allocate(n);
    }

    void deallocate(T* p, size_t n)
    {
        data_allocator_::deallocate(p, n);
    }
};

template <typename T, typename Alloc = Allocator<T>>
class vector : protected VectorBase<T, Alloc>
{
protected:
    using BaseType = VectorBase<T, Alloc>;

public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using iterator = value_type*;
    using const_iterator = const value_type*;

    using const_reverse_iter = reverse_iterator<const_iterator>;
    using reverse_iter = reverse_iterator<iterator>;
    using allocator_type = typename BaseType::allocator_type;
    allocator_type get_allocator() const { return allocator_type::get_allocator(); }

    using BaseType::allocate;
    using BaseType::deallocate;
    using BaseType::end_of_storage_;
    using BaseType::finish_;
    using BaseType::start_;

    // 构造函数
    vector(const allocator_type& alloc = allocator_type()) : BaseType(alloc) {}

    vector(size_type n) : BaseType(n, allocator_type())
    {
        finish_ = uninitialized_fill_n(start_, n, value_type());
    }

    vector(size_type n, const_reference value,
           const allocator_type& alloc = allocator_type()) : BaseType(n, alloc)
    {
        finish_ = uninitialized_fill_n(start_, n, value);
    }

    // 拷贝构造
    vector(const vector<T, Alloc>& another) : BaseType(another.size(), another.get_allocator())
    {
        finish_ = uninitialized_copy(another.begin(), another.end(), start_);
    }

    ~vector() { destroy(start_, finish_); }

    // 由迭代器[first,last)构造
    // 这里要判断 InputIterator 是否是一个整数， 与vector(size, value)做区分。
    template <typename InputIterator>
    vector(InputIterator first, InputIterator last,
           const allocator_type& alloc = allocator_type()) : BaseType(alloc)
    {
        using is_interger = typename __is_integer<InputIterator>::is_integer;
        initialize_with_iterator(first, last, is_interger());
    }

protected:
    template <typename Integer>
    void initialize_with_iterator(Integer n, Integer value, __true_type)
    {
        start_ = allocate(n);
        finish_ = uninitialized_fill_n(start_, n, value);
        end_of_storage_ = start_ + n;
    }

    template <typename InputIterator>
    void initialize_with_iterator(InputIterator first, InputIterator last, __false_type)
    {
        range_initialize(first, last, __ITERATOR_CATEGORY(first));
    }

    template <typename InputIterator>
    void range_initialize(InputIterator first, InputIterator last, input_iterator_tag)
    {
        for (; first != last; ++first)
            push_back(*first);
    }

    template <typename ForwardIter>
    void range_initialize(ForwardIter first, ForwardIter last, forward_iterator_tag)
    {
        size_type n = distance(first, last);
        start_ = allocate(n);
        finish_ = uninitialized_copy(first, last, start_);
        end_of_storage_ = start_ + n;
    }

public:
    iterator begin() { return start_; }
    const_iterator cbegin() const { return begin(); }
    iterator end() { return finish_; }
    const_iterator cend() const { return end(); }

    reverse_iter rbegin() { return reverse_iter(end()); }
    const_reverse_iter crbegin() const { return rbegin(); }
    reverse_iter rend() { return reverse_iter(begin()); }
    const_reverse_iter crend() const { return rend(); }

    size_type size() { return finish_ - start_; }
    size_type max_size() { return size_type(-1) / sizeof(value_type); }
    size_type capacity() { return end_of_storage_ - start_; }

    reference operator[](size_t n) { return *(begin() + n); }
    const_reference operator[](size_t n) const { return *(begin() + n); }

    // TODO 异常处理
    reference at(size_t n) { return (*this)[n]; }
    const_reference at(size_t n) const { return (*this)[n]; }

    reference front() { return *begin(); }
    const_reference front() const { return front(); }
    reference back() { return *(end() - 1); }
    const_reference back() const { return back(); }

    vector<T, Alloc>& operator=(const vector<T, Alloc>& another) { return *this; }

    // 只有当 reserve 的size > capacity的时候调用才有效
    void reserve(size_type n)
    {
        if (n > capacity())
        {
            const size_type old_size = size();
            // 申请更大的空间，将原对象copy过来
            iterator tmp = allocate_and_copy(n, start_, finish_);
            // 释放原对象
            destroy(start_, finish_);
            // 释放原空间
            deallocate(start_, end_of_storage_ - start_);
            start_ = tmp;
            finish_ = tmp + old_size;
            end_of_storage_ = start_ + n;
        }
    }
    void assign(size_type n, const_reference val) {}
    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last) {}

    void push_back(const T& value) {}
    void push_back(const T&& value) {}

    // void emplace_back() {}
    // void shrink_to_fit(){}

    void swap(vector<T, Alloc>& another) {}

    iterator insert(iterator position, const T& value) {}
    iterator insert(iterator position, size_type n, const T& value) {}
    template <typename InputIterator>
    iterator insert(iterator position, InputIterator first,
                    InputIterator last) {}

    // iterator emplace(){}

    void pop_back() {}
    iterator erase(iterator position) {}
    iterator earse(iterator first, iterator last) {}

    void resize(size_type new_size, const T& value) {}
    void resize(size_type new_size) {}
    void clear() {}

protected:
    template <typename ForwardIterator>
    iterator allocate_and_copy(size_type n, ForwardIterator first, ForwardIterator last)
    {
        // TODO exception
        iterator result = allocate(n);
        uninitialized_copy(first, last, result);
        return result;
    }
};

} // namespace hl

#endif

