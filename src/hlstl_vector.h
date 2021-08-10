#ifndef HLSTL_VECTOR_H
#define HLSTL_VECTOR_H

#include "hlstl_alloc.h"
#include "hlstl_allocator.h"
#include "hlstl_iterator.h"

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

    // TODO 由迭代器[first,last)构造
    // template <class InputerIter>
    // vector(InputerIter first, InputerIter last,
    //        const allocator_type& alloc = allocator_type()) : BaseType(alloc)
    // {
    // }

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

    vector<T, Alloc>& operator=(const vector<T, Alloc>& another) { return *this; }
    void reserve(size_type n) {}
    void assign(size_type n, const_reference val) {}
    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last) {}

    reference front() { return *begin(); }
    const_reference front() const { return front(); }
    reference back() { return *(end() - 1); }
    const_reference back() const { return back(); }

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
};

} // namespace hl

#endif
